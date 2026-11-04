# 11 — Focused C++20 Technical Architecture

**Status:** Design specification, **not implemented code**.  
**Updated:** 2026-11-04  
**Primary goal:** minimum viable DV01-balanced 2Y–10Y stress study.  
**Limits:** No ML, live trading, historical portfolio ledger or financing engine in the MVP.

## 1. Design principle

**Build five small modules, in order.** Do not implement the optional curve/data modules until pricing, sensitivities and scenario outputs pass their tests.

- **Core modules:** `Bond`, `BondPricing`, `BondRisk`, `CurveTrade`, `ScenarioAnalysis`.
- **Optional after completion:** `YieldCurve` for zero-rate discounting, and `MarketData` for descriptive Treasury par-yield history.
- **Do not add in this eight-week project:** `Backtester`, `CashLedger`, `Financing`, `DatedBond`, `Performance`, a database or a trading API.

Use RAII/`std::vector`, `double` for numerical rates and prices, `const` and explicit input validation. Prefer plain structs for inputs/outputs and free mathematical functions when no mutable state is needed.

## 2. Target directory structure

```text
Treasury_Curve_Risk_CPP/
├── README.md                         [EXISTS]
├── PROJECT_STATUS.md                 [EXISTS]
├── CMakeLists.txt                    [EXISTS — extend phase by phase]
├── .gitignore                        [EXISTS]
├── project_manifest.json            [EXISTS]
│
├── include/
│   ├── README.md                     [EXISTS]
│   └── treasury/
│       ├── README.md                 [THIS RESTRUCTURE]
│       ├── Bond.h                    [PHASE 1 — PLANNED]
│       ├── BondPricing.h             [PHASES 1–2 — PLANNED]
│       ├── BondRisk.h                [PHASE 3 — PLANNED]
│       ├── CurveTrade.h              [PHASE 4 — PLANNED]
│       ├── ScenarioAnalysis.h        [PHASE 5 — PLANNED]
│       ├── YieldCurve.h              [OPTIONAL]
│       └── MarketData.h              [OPTIONAL]
├── src/
│   ├── main.cpp                      [EXISTS — placeholder]
│   ├── Bond.cpp                      [PHASE 1 — PLANNED]
│   ├── BondPricing.cpp               [PHASES 1–2 — PLANNED]
│   ├── BondRisk.cpp                  [PHASE 3 — PLANNED]
│   ├── CurveTrade.cpp                [PHASE 4 — PLANNED]
│   ├── ScenarioAnalysis.cpp          [PHASE 5 — PLANNED]
│   ├── YieldCurve.cpp                [OPTIONAL]
│   └── MarketData.cpp                [OPTIONAL]
├── tests/
│   ├── README.md                     [EXISTS]
│   ├── test_bond.cpp                 [PHASE 1 — PLANNED]
│   ├── test_pricing.cpp              [PHASES 1–2 — PLANNED]
│   ├── test_risk.cpp                 [PHASE 3 — PLANNED]
│   ├── test_trade.cpp                [PHASE 4 — PLANNED]
│   └── test_scenarios.cpp            [PHASE 5 — PLANNED]
├── data/
│   ├── sample/
│   │   ├── known_price_cases.csv     [EXISTS]
│   │   └── illustrative_trade_scenarios.csv [THIS RESTRUCTURE]
│   ├── raw/README.md                 [EXISTS — optional]
│   └── processed/README.md           [EXISTS — optional]
├── docs/
│   ├── 01_project_charter.md
│   ├── 02_math_and_conventions.md
│   ├── 03_trading_strategy.md
│   ├── 04_data_plan.md
│   ├── 05_backtest_methodology.md
│   ├── 06_validation_plan.md
│   ├── 07_eight_week_roadmap.md
│   ├── 08_reading_map.md
│   ├── 09_decision_log.md
│   ├── 10_final_report_outline.md
│   ├── 11_technical_architecture.md
│   └── 12_start_here.md
├── python/README.md                  [EXISTS — optional]
└── results/README.md                 [EXISTS — generated later]
```

**Important:** The structure distinguishes **currently existing files from future files**. Headers and sources should not be created as empty placeholders. Create each pair when coding its phase, keep CMake buildable throughout, and add a test immediately.

## 3. Core file-by-file API submap

### 3.1 `Bond.h` and `Bond.cpp` (Phase 1)

A bond object owns **specification values**, not manual heap arrays.

```cpp
namespace treasury {

struct CashFlow {
    double timeYears;
    double amountUsd;
};

class Bond {
private:
    double faceValueUsd_;
    double annualCouponRate_; // decimal
    double maturityYears_;
    int couponsPerYear_;

public:
    Bond(double faceValueUsd,
         double annualCouponRate,
         double maturityYears,
         int couponsPerYear);

    double faceValueUsd() const noexcept;
    double annualCouponRate() const noexcept;
    double maturityYears() const noexcept;
    int couponsPerYear() const noexcept;

    std::vector<CashFlow> cashFlows() const;
};

}
```

- `Bond()` validates positive face, nonnegative coupon, positive frequency and a maturity that represents an **integer number of regular coupon periods**.
- `cashFlows()` returns `N = maturityYears * couponsPerYear` payments at `k/frequency` years, each coupon `face * rate / frequency`; final payment also includes face.
- No calendar dates, accrued interest, floating coupons or settlement mechanics in the MVP.
- **Test** `test_bond.cpp`: for 2Y, $100 face, 4%, semiannual -> cash flows `(2,2,2,102)`.

### 3.2 `BondPricing.h` and `BondPricing.cpp` (Phases 1–2)

Mathematical operations are **free functions** rather than a mutable pricer class.

```cpp
namespace treasury {

double priceFromYTM(const Bond& bond, double annualYtmDecimal);

double solveYTMFromPrice(const Bond& bond,
                         double targetPriceUsd,
                         double lowerYieldDecimal,
                         double upperYieldDecimal,
                         double tolerance = 1e-12,
                         int maxIterations = 200);

}
```

- `priceFromYTM()`: calculate `P = SUM CF_k / (1+y/m)^k`; require positive compounding base.
- `solveYTMFromPrice()`: bisection on a valid bracket, terminate on yield interval and/or price residual, fail clearly when unbracketed/nonconvergent.
- **Test** `test_pricing.cpp`: `P(5%) ≈ 98.1190129`, `P(4%)=100`, zero coupon, yield monotonicity, recover 5% from a known **unrounded** price, invalid inputs.

### 3.3 `BondRisk.h` and `BondRisk.cpp` (Phase 3)

```cpp
namespace treasury {

struct BondRiskMetrics {
    double macaulayDurationYears;
    double modifiedDurationYears;
    double convexityYearsSquared;
    double dv01UsdPerBp;  // for bond.faceValueUsd()
};

BondRiskMetrics calculateRisk(const Bond& bond, double annualYtmDecimal);
double dv01FiniteDifference(const Bond& bond,
                            double annualYtmDecimal,
                            double bumpDecimal = 0.0001);
double approximatePriceChange(const Bond& bond,
                              double annualYtmDecimal,
                              double yieldMoveDecimal,
                              bool includeConvexity = false);

}
```

- `calculateRisk()` evaluates Macaulay/modified duration, convexity and positive DV01 magnitude for conventional fixed-coupon cash flows.
- `dv01FiniteDifference()` checks `[P(y-1bp)-P(y+1bp)]/2` = dollars per **1bp**; do not divide by 2h when reporting dollars/bp.
- `approximatePriceChange()` gives the negative-duration term, optionally adding the positive-convexity quadratic term.
- **Test:** independently compare formulas and finite differences; check dollar/bp units, notional scaling and sensitivity signs.

### 3.4 `CurveTrade.h` and `CurveTrade.cpp` (Phase 4)

A compact trade representation for **two existing** `Bond` objects (or copies) and **signed face notionals**.

```cpp
namespace treasury {

struct TradeLeg {
    Bond bond;
    double startingYtmDecimal;
    double signedFaceNotionalUsd; // positive long, negative short
};

struct CurveTrade {
    TradeLeg twoYear;
    TradeLeg tenYear;
};

CurveTrade makeDv01Steepener(
    const Bond& bond2y, double ytm2y,
    const Bond& bond10y, double ytm10y,
    double anchorTwoYearFaceUsd);

CurveTrade makeDv01Flattener(
    const Bond& bond2y, double ytm2y,
    const Bond& bond10y, double ytm10y,
    double anchorTwoYearFaceUsd);

double signedParallelDv01UsdPerBp(const CurveTrade& trade);

}
```

- Compute `perFaceDV01UsdPerBp` consistently for each bond; hedge magnitudes using the ratio of **dollar DV01 per unit face**.
- For a steepener `signedFace2y > 0` and `signedFace10y < 0`; reverse both for a flattener.
- `signedParallelDv01UsdPerBp()` reports the **signed** P&L impact per +1bp on both yields (near zero for matched legs).
- **Test:** matched initial risk, correct long/short signs, consistent scaling. The midpoint-slide `$190/$760` example is **an assumed reference**, not a required output of `calculateRisk()`.

### 3.5 `ScenarioAnalysis.h` and `ScenarioAnalysis.cpp` (Phase 5)

```cpp
namespace treasury {

struct RateScenario {
    const char* name;  // static label for MVP scenarios
    double delta2yBp;
    double delta10yBp;
};

struct ScenarioResult {
    double firstOrderPnlUsd;
    double fullRepricingPnlUsd;
    double approximationErrorUsd;
};

ScenarioResult evaluateScenario(
    const CurveTrade& trade,
    const RateScenario& shock);

}
```

- `firstOrderPnlUsd` uses signed position DV01s multiplied by each scenario yield change in **basis points**.
- `fullRepricingPnlUsd` sums signed notional-scaled differences in `priceFromYTM` at shocked versus original YTM.
- `approximationErrorUsd` = full repricing minus first-order approximation.
- **Important:** shock both bond YTMs with unchanged cash flows. **Do not treat this as zero-curve repricing** or a historical trading strategy.
- **Test:** parallel +10/+10bp (first order ≈ 0), steepening +5/+15bp (positive first order for steepener), flattening +15/+5bp (negative first order); exact repricing may leave a convexity residual.

### 3.6 `main.cpp` (integration and output)

- Construct one hypothetical 2Y and one 10Y bond (record chosen coupons and initial yields).
- Price both, calculate risk, build a DV01-balanced steepener, run the three scenarios, and print a table containing scenario name, 2Y/10Y moves, DV01 P&L, exact P&L and approximation error.
- Print chosen face notionals, signed DV01s, rate conventions and scenario-only disclaimer.
- When ready, also show a flattener by reversing both trade signs.
- Do not add a GUI, broker interface or command-line argument parser until core tests pass.

## 4. Optional module interfaces (NOT mandatory)

### `YieldCurve.h/.cpp` — stretch A

A small `YieldCurve` class may hold `vector<ZeroRateNode>` with maturity and **zero rate** (not Treasury par yield) and expose `discountFactor(t)` and `priceFromDiscountCurve(bond)`. State compounding/interpolation and check every coupon-date discount. Do **not** silently use 2Y and 10Y par yields as a complete zero curve.

### `MarketData.h/.cpp` — stretch B

Load dated **par** 2Y/10Y Treasury yield observations from a sourced CSV, validate units/missing dates, and calculate `spreadBp = (y10 - y2) * 10000`. Keep history **descriptive**; do not derive portfolio returns from a spread chart.

Do not create optional source files until the five required core components are complete, tested and documented.

## 5. CMake/CTest plan

Current `CMakeLists.txt` builds `src/main.cpp` as `treasury_curve_cli`. Add new source files **as they are implemented**. The eventual design should have a reusable library with public include path and tests.

```cmake
add_library(treasury_core
    src/Bond.cpp
    src/BondPricing.cpp
    src/BondRisk.cpp
    src/CurveTrade.cpp
    src/ScenarioAnalysis.cpp
)
target_include_directories(treasury_core PUBLIC include)
target_link_libraries(treasury_curve_cli PRIVATE treasury_core)

enable_testing()
# Add each test executable and add_test(...) when its source exists.
```

**This fragment is the future target, not valid for the current scaffold until those source files exist.** Keep CMake buildable at every intermediate commit.

## 6. Dependency flow

```text
Bond (contract and cash flows)
  └── BondPricing (PV and YTM root solver)
         └── BondRisk (duration, convexity and DV01)
                └── CurveTrade (long/short face weights)
                       └── ScenarioAnalysis (first-order vs exact P&L)
                              └── main.cpp (demo output)
```

Each module depends only on previously completed modules. Avoid bidirectional links. Tests mirror the same ladder.

## 7. Eight development phases (concrete gates)

| Week | Work | Must pass before continuing |
|---|---|---|
| 1 | Bond + price function | Cash-flow and benchmark price tests |
| 2 | Bisection YTM | Root and failure-path tests |
| 3 | BondRisk | Sensitivity unit and finite-difference tests |
| 4 | CurveTrade | Signed-DV01 neutrality and hedge-size tests |
| 5 | ScenarioAnalysis | Parallel/steepener/flattener P&L tests |
| 6 | CTest and robustness | Clean build with regression tests |
| 7 | Results/report draft | Reproducible output and risk interpretation; history optional |
| 8 | Final integration | Final presentation and documented limitations |

## 8. Deliberately deferred features

**Full synthetic historical backtesting** would add dated cash-flow schedules, position-aging, clean/dirty prices, transaction timestamps, short coupon obligations, funding/borrow/collateral accounts, and reproducible market curves. That belongs in a **separate next project**.

No Markov switching, machine learning, live order routing or profit prediction is included.

## 9. Acceptance and file links

- [Start here — first implementation session](12_start_here.md)
- [Price and risk formulas](02_math_and_conventions.md)
- [Trade signs and midpoint illustration](03_trading_strategy.md)
- [Independent test specifications](06_validation_plan.md)
- [Eight-week schedule](07_eight_week_roadmap.md)
- [Project status](../PROJECT_STATUS.md)

**First price benchmark:** $100 face, 4% annual coupon, 2 years, semiannual YTM 5% → about **$98.1190129**. This is a **theoretical benchmark**, not evidence that the C++ tests have passed.
