# Treasury Yield Curve Trading and Risk Analysis in C++

## 11 — Technical Architecture: Focused C++20 Core

**Document status:** proposed implementation specification, **not completed code**  
**Revision:** November 4, 2026  
**Repository:** `sebastianwin01/Treasury_Curve_Risk_CPP`  
**Research scope:** fixed-coupon Treasury-style bond pricing and interest-rate risk; DV01-balanced 2Y–10Y curve trades; deterministic stress analysis; **conditional synthetic historical research**. **No machine learning, Markov-switching estimation, forecasting, or live trading.**

This replaces the earlier, larger architecture proposal with a smaller design that fits the FQE midpoint presentation and the project's approximately **eight-week, 5–7-hours/week** plan. It preserves the previously agreed **eight development phases**, but starts with **seven core headers and seven matching source files** rather than adding a class for every formula or accounting operation.

> **Status legend:** `[E]` = exists in the reviewed GitHub repository; `[N]` = proposed new source or document; `[C]` = conditional file or generated output (do not create until needed). The status of a folder alone does not imply its planned implementation exists. The reviewed `src/main.cpp` is a console placeholder, not a functioning financial engine.

---

## 1. Target directory structure

```text
Treasury_Curve_Risk_CPP/
│
├── README.md                                  [E] Project summary, scope, phases
├── PROJECT_STATUS.md                          [E] Evidence-based milestone tracking
├── CMakeLists.txt                              [E] C++20 build; extend as modules arrive
├── .gitignore                                 [E] Ignore build and generated data
├── project_manifest.json                      [E] Machine-readable project metadata
│
├── docs/
│   ├── 01_project_charter.md                   [E] Research objective and success criteria
│   ├── 02_math_and_conventions.md              [E] Pricing and sensitivity formulas
│   ├── 03_trading_strategy.md                  [E] Steepener/flattener definitions
│   ├── 04_data_plan.md                         [E] Data sources and rate distinctions
│   ├── 05_backtest_methodology.md              [E] Synthetic accounting assumptions
│   ├── 06_validation_plan.md                   [E] Independent acceptance tests
│   ├── 07_eight_week_roadmap.md                [E] Eight-phase schedule
│   ├── 08_reading_map.md                       [E] Tuckman/Serrat references
│   ├── 09_decision_log.md                      [E] Confirmed vs. open conventions
│   ├── 10_final_report_outline.md             [E] Final research report plan
│   └── 11_technical_architecture.md           [N] This focused architecture
│
├── include/
│   ├── README.md                               [E] Existing placeholder
│   └── treasury/
│       ├── Bond.h                              [N] Bond and cash-flow value types
│       ├── BondPricing.h                       [N] Price and YTM-solver functions
│       ├── BondRisk.h                          [N] Duration, convexity, DV01
│       ├── YieldCurve.h                        [N] Zero-curve and discount factors
│       ├── CurveTrade.h                        [N] Positions and DV01-neutral sizing
│       ├── ScenarioAnalysis.h                  [N] Curve shocks and scenario P&L
│       └── MarketData.h                        [N] Data types and CSV loaders
│
├── src/
│   ├── main.cpp                                [E] CLI; currently a placeholder
│   ├── Bond.cpp                               [N] Cash-flow generation and validation
│   ├── BondPricing.cpp                        [N] PV, YTM bisection, curve pricing
│   ├── BondRisk.cpp                           [N] Analytical and bumped risk measures
│   ├── YieldCurve.cpp                         [N] Interpolation and curve shocks
│   ├── CurveTrade.cpp                         [N] Leg exposure and hedge construction
│   ├── ScenarioAnalysis.cpp                   [N] Full repricing versus approximations
│   └── MarketData.cpp                         [N] CSV parsing and source validation
│
├── tests/
│   ├── README.md                               [E] Existing placeholder
│   ├── test_bond.cpp                          [N] Cash-flow amount and validation
│   ├── test_pricing.cpp                       [N] Price, YTM and numerical convergence
│   ├── test_risk.cpp                          [N] Duration, convexity, DV01
│   ├── test_curve.cpp                         [N] Discounting and interpolation
│   ├── test_trades.cpp                        [N] Hedge ratios and scenario P&L
│   └── test_market_data.cpp                   [C] CSV formats and missing-rate cases
│
├── data/
│   ├── sample/
│   │   ├── known_price_cases.csv              [E] Independent bond-price benchmarks
│   │   ├── sample_zero_curve.csv              [N] Tiny hand-checked discount curve
│   │   └── sample_2y10y_shocks.csv            [N] Illustrative shock cases
│   ├── raw/
│   │   └── README.md                          [E] Source documentation
│   └── processed/
│       └── README.md                          [E] Normalization documentation
│
├── python/
│   ├── README.md                               [E] Optional non-ML helpers
│   ├── prepare_treasury_data.py               [C] Clean/download permitted data
│   └── plot_results.py                        [C] Figures from C++ result CSVs
│
└── results/
    ├── README.md                              [E] Output conventions
    ├── scenario_results.csv                   [C] Generated deterministic scenarios
    ├── synthetic_backtest.csv                 [C] Only if viable historical model exists
    └── figures/                               [C] Generated plots
```

**Scope control:** No `YieldSolver.h`, `BondCashFlows.h`, `Position.h`, `TradeSizing.h`, `CurveShock.h`, `Performance.h`, or `ResultWriter.h` is required in the **core**. Their small responsibilities live in the seven modules above. If those modules become unwieldy, splitting them later is allowed, but is not a milestone requirement.

---

## 2. General design contracts

1. **Namespace:** `treasury`. Include directives in clients use e.g. `#include "treasury/Bond.h"` after `include/` is added to the CMake target's public include path.
2. **C++ style:** simple value types, `std::vector`, `std::string`, standard algorithms and RAII. Do **not** use owning `new[]`/`delete[]` arrays. Prefer pure numerical functions over mutable state.
3. **Member conventions:** private class members end in `_`; functions use consistent `camelCase`; `.h` declares interfaces and `.cpp` implements nontrivial methods.
4. **Rate units:** annual yields and coupon rates are **decimal** (`0.05` = 5%); rate changes explicitly named `...Bp` are **basis points** (`1 bp = 0.0001` in decimal yield).
5. **Money and duration:** price is USD **per `Bond::faceValueUsd()` quotation unit** (initially $100). Position notionals are actual **face USD**. Multiply a per-unit price or DV01 by `position.faceAmountUsd / bond.faceValueUsd()`; duration is in years, convexity is in years squared, DV01 in USD/bp.
6. **First bond abstraction:** regular equal coupon periods with `maturityYears * couponsPerYear` a positive integer. This simplified model is **not sufficient** for an actual dated Treasury settlement or coupon/holiday calendar.
7. **Risk sign:** `dv01UsdPerBp` denotes a **positive sensitivity magnitude** for a conventional long bond with positive cash flows. Price P&L for a long leg is approximately `-DV01 * deltaYieldBp`; a short reverses sign.
8. **Rate type:** a par constant-maturity rate is **not** a zero rate, cash-flow discount factor, security YTM or trade quote. `YieldCurve` receives validated **zero-rate/discount-compatible** nodes only.
9. **Historical integrity:** a bond's coupon and final maturity remain fixed after simulated entry; it **ages**. Continuous rolling to new constant-maturity instruments must be modeled as explicit transactions.
10. **Evidence:** this is an interface plan. Methods, tests, benchmark results and charts are **planned**, not asserted to have run. Update `PROJECT_STATUS.md` only when results can be reproduced.

---

## 3. Core modules — data members and method signatures

These C++ snippets define the **intended APIs**. They are not yet committed, and each header will need its normal includes/forward declarations and namespace guards (`#pragma once` is fine). Some defaults may be adjusted when coding and independently validating the formulas.

### 3.1 `Bond.h` / `Bond.cpp` — Phase 1

**Responsibility:** represent one simple fixed-rate coupon bond and generate its cash-flow schedule. **Do not** make the bond class responsible for portfolio strategy, current market yields or backtest accounting.

```cpp
#pragma once
#include <vector>

namespace treasury {

struct CashFlow {
    double timeYears{};     // From the valuation time, in years
    double amountUsd{};     // Per Bond quotation unit (e.g., $100 face)
};

class Bond {
private:
    double faceValueUsd_{};
    double annualCouponRate_{};  // Decimal: 0.04 = 4%
    double maturityYears_{};
    int couponsPerYear_{};

public:
    Bond(double faceValueUsd, double annualCouponRate,
         double maturityYears, int couponsPerYear);

    double faceValueUsd() const noexcept;
    double annualCouponRate() const noexcept;
    double maturityYears() const noexcept;
    int couponsPerYear() const noexcept;

    std::vector<CashFlow> regularCashFlows() const;
};

} // namespace treasury
```

**Implementation details:** constructor validates positive finite face, nonnegative finite coupon for the baseline, positive finite maturity, and positive integer frequency; first version requires an integer number of regular coupon periods. A payment at period `k` has time `k/m` and coupon amount `F*c/m`, with `F` principal added to the final cash flow.

**Example:** `Bond(100.0, 0.04, 2.0, 2)` creates payments of `$2, $2, $2, $102` at `0.5, 1.0, 1.5, 2.0` years. This class owns only basic values; **no custom destructor, copy/move constructor or copy/move assignment** is necessary.

### 3.2 `BondPricing.h` / `BondPricing.cpp` — Phases 1–3

**Responsibility:** numerical valuation and a simple bracketed YTM solver. This module has **no persistent data members**.

```cpp
#pragma once
#include "treasury/Bond.h"

namespace treasury {

class YieldCurve;  // Defined in YieldCurve.h

struct YtmSolverSettings {
    double lowerYieldDecimal{-0.02}; // Example bracket; validated for m
    double upperYieldDecimal{0.50};
    double priceToleranceUsd{1e-10};
    int maxIterations{200};
};

struct YtmSolveResult {
    double yieldDecimal{};
    double priceResidualUsd{};
    int iterations{};
};

// Phase 1: present value at a nominal annual yield compounded m times/year
// Price is USD per bond quotation unit.
double priceFromYTM(const Bond& bond, double ytmDecimal);

// Phase 2: solve priceFromYTM(bond, y) = marketPriceUsd by bisection.
YtmSolveResult solveYTM(const Bond& bond, double marketPriceUsd,
                        const YtmSolverSettings& settings = {});

// Phase 3: discount each coupon/principal CF using its maturity-specific D(t).
double priceFromCurve(const Bond& bond, const YieldCurve& curve);

} // namespace treasury
```

**Finance equation, Phase 1:**

$$
P(y)=\sum_{k=1}^{N}\frac{CF_k}{(1+y/m)^k}
$$

`solveYTM()` checks that the price is positive, the initial bracket is valid and contains a sign change, compounding bases are positive, tolerances are finite and positive, and the solver has converged before returning. The implementation may throw a descriptive `std::invalid_argument` or `std::runtime_error` instead of silently returning a bad yield.

`priceFromCurve()` is declared up front but **implemented only in Phase 3**; it uses `Bond::regularCashFlows()` and `YieldCurve::discountFactor()`. Curve prices here are hypothetical values per bond quotation unit, not executed Treasury quotes.

### 3.3 `BondRisk.h` / `BondRisk.cpp` — Phases 2–4

**Responsibility:** measure interest-rate sensitivities and check them against numerical repricing. No persistent class state.

```cpp
#pragma once
#include "treasury/Bond.h"

namespace treasury {
class YieldCurve;

struct BondRiskMetrics {
    double macaulayDurationYears{};
    double modifiedDurationYears{};
    double convexityYearsSquared{};
    double dv01UsdPerBp{};  // Magnitude, per Bond quotation unit
};

BondRiskMetrics calculateBondRisk(const Bond& bond, double ytmDecimal);

double dv01FiniteDifference(const Bond& bond, double ytmDecimal,
                            double bumpBp = 1.0);

double approximatePriceChange(const Bond& bond, double ytmDecimal,
                              double yieldChangeBp, bool includeConvexity);

// Phase 4: parallel bump every zero rate in the same curve convention.
double parallelCurveDV01(const Bond& bond, const YieldCurve& curve,
                         double bumpBp = 1.0);

} // namespace treasury
```

**Central definitions:**

$$
D_{mod}=-\frac{1}{P}\frac{dP}{dy},\qquad
DV01=-\frac{dP}{dy}\times 0.0001,\qquad
C=\frac{1}{P}\frac{d^2P}{dy^2}
$$

**Tests:** analytic duration and convexity versus central differences; DV01 per `$100` face versus scaled `$1m` positions; plus/minus 1-bp and larger shocks. A `parallelCurveDV01` uses **curve** price shocks; it should not silently mix YTM-bump DV01 with curve-bump DV01.

### 3.4 `YieldCurve.h` / `YieldCurve.cpp` — Phase 3

**Responsibility:** hold the *cross-sectional zero-rate curve* used to discount coupon cash flows. The first version is not a dynamic Nelson–Siegel, Markov or predictive model.

```cpp
#pragma once
#include <vector>

namespace treasury {

struct ZeroCurveNode {
    double maturityYears{};
    double zeroRateDecimal{};
};

enum class ZeroCompounding { Continuous, Semiannual };
enum class CurveInterpolation { LinearZeroRate, LinearLogDiscount };

class YieldCurve {
private:
    std::vector<ZeroCurveNode> nodes_;   // Strictly increasing maturities
    ZeroCompounding compounding_;
    CurveInterpolation interpolation_;

public:
    YieldCurve(std::vector<ZeroCurveNode> nodes,
               ZeroCompounding compounding,
               CurveInterpolation interpolation);

    double zeroRate(double maturityYears) const;
    double discountFactor(double maturityYears) const;
    YieldCurve shiftedParallelBp(double shiftBp) const;
    YieldCurve shiftedTwoTenBp(double twoYearShiftBp,
                               double tenYearShiftBp) const;
    const std::vector<ZeroCurveNode>& nodes() const noexcept;
};

} // namespace treasury
```

**Finance equation, Phase 3:**

$$
P_{curve}=\sum_{k=1}^{N}CF_k D(t_k)
$$

For **continuous** compounding only, `D(t) = exp(-z(t)*t)`. For semiannual zero rates, apply that explicit compounding convention instead; never mix them. Interpolation should be specified and tested; values outside provided maturities must have a deliberate boundary policy, not silent extrapolation.

**Shock convention (proposed, not locked):** `shiftedTwoTenBp()` applies the specified 2Y and 10Y shifts at those nodes, interpolates **shifts** linearly between 2 and 10 years and applies the nearer anchor shift outside that range. Confirm this convention in `docs/09_decision_log.md` before using it for conclusions.

### 3.5 `CurveTrade.h` / `CurveTrade.cpp` — Phase 4

**Responsibility:** represent the two positions, calculate notional scaling and construct DV01-balanced steepeners/flatteners. This **combines** the earlier proposed `Position.h` and `TradeSizing.h` responsibilities.

```cpp
#pragma once
#include "treasury/Bond.h"

namespace treasury {
class YieldCurve;

enum class PositionSide { Long, Short };

struct TradeLeg {
    Bond bond;
    PositionSide side;
    double faceAmountUsd;  // Actual face notional, >= 0
};

class CurveTrade {
private:
    TradeLeg twoYear_;
    TradeLeg tenYear_;

public:
    CurveTrade(TradeLeg twoYear, TradeLeg tenYear);

    const TradeLeg& twoYear() const noexcept;
    const TradeLeg& tenYear() const noexcept;

    // Use the same curve-DV01 definition for both sides.
    static CurveTrade makeDv01NeutralSteepener(
        const Bond& twoYearBond, const Bond& tenYearBond,
        const YieldCurve& curve, double twoYearFaceAmountUsd);

    static CurveTrade makeDv01NeutralFlattener(
        const Bond& twoYearBond, const Bond& tenYearBond,
        const YieldCurve& curve, double twoYearFaceAmountUsd);

    double netParallelDV01(const YieldCurve& curve) const;
};

} // namespace treasury
```

**Position scaling:** Let each per-quote-unit DV01 be positive for a conventional long bond; then the DV01 per **one face dollar** is `dv01Unit / bond.faceValueUsd()`.

For a chosen 2Y **face notional** `q2`, the 10Y notional that matches magnitudes is:

$$
q_{10}=q_2\frac{DV01_2/F_2}{DV01_{10}/F_{10}}
$$

where `F2` and `F10` are each instrument's face quotation units. To compute net parallel DV01, **long = +DV01** and **short = −DV01**. For a yield increase, first-order P&L is `− net_signed_DV01 × change_in_bp`.

**Illustrative midpoint example, not live data:** 2Y `$190/bp per $1m` and 10Y `$760/bp per $1m` gives **long $4m 2Y / short $1m 10Y**. Each leg has `$760/bp` of absolute risk. These particular figures are benchmark *inputs*, not outputs of actual market pricing.

### 3.6 `ScenarioAnalysis.h` / `ScenarioAnalysis.cpp` — Phase 5

**Responsibility:** apply chosen interest-rate shocks to the full portfolio and compare DV01 approximations against revaluation. This **combines** the earlier `CurveShock.h` and `ScenarioAnalysis.h` concepts.

```cpp
#pragma once
#include <string>
#include "treasury/CurveTrade.h"

namespace treasury {
class YieldCurve;

struct CurveScenario {
    std::string name;
    double twoYearShiftBp{};
    double tenYearShiftBp{};
};

struct ScenarioResult {
    std::string name;
    double firstOrderPnlUsd{};
    double fullRepricingPnlUsd{};
    double approximationErrorUsd{};
    double netParallelDv01UsdPerBp{};
};

ScenarioResult evaluateScenario(const CurveTrade& trade,
                                const YieldCurve& baseCurve,
                                const CurveScenario& scenario);

} // namespace treasury
```

**Algorithm:** price each leg at the original curve; construct a shocked curve using the **documented shift interpolation**; reprice each leg; multiply price changes by signed face-amount ratios; compare exact model P&L with first-order DV01 P&L under an explicitly consistent shock basis. The `approximationErrorUsd` convention must be stated (recommended: `fullRepricingPnlUsd − firstOrderPnlUsd`).

**Required sample scenarios:** parallel `(+10,+10) bp`, steepening `(+5,+15) bp`, flattening `(+15,+5) bp`, plus larger moves. For the **midpoint's assumed $760/bp per-leg exposure**, first-order P&L is `$0`, `+$7,600`, and `−$7,600`, respectively. Because a real coupon bond has curve and convexity exposures, a full curve-repricing result **need not** exactly reproduce the simplistic two-yield formula.

### 3.7 `MarketData.h` / `MarketData.cpp` — Phase 6

**Responsibility:** load and validate dated observations. Do not use CSV import as a hidden rate conversion or assume a par-yield series is a zero-rate curve.

```cpp
#pragma once
#include <string>
#include <vector>
#include "treasury/YieldCurve.h"

namespace treasury {

struct ParYieldObservation {
    std::string dateIso;     // YYYY-MM-DD
    double twoYearRateDecimal{};
    double tenYearRateDecimal{};
};

struct ZeroCurveObservation {
    std::string dateIso;
    std::vector<ZeroCurveNode> nodes;
};

std::vector<ParYieldObservation> loadParYieldCsv(const std::string& path);
std::vector<ZeroCurveObservation> loadZeroCurveCsv(const std::string& path);

} // namespace treasury
```

**Checks:** enforce a documented CSV schema, parse valid ISO dates, normalize explicit source units once, reject duplicate dates/tenors unless a recorded rule resolves them, report missing 2Y/10Y pairs and disallow negative or invalid maturity nodes. For curve-based valuation, preserve the **source's actual zero-rate compounding** and create an explicitly configured `YieldCurve`; if the data are par yields only, produce **descriptive slope statistics**, not a fabricated curve backtest.

---

## 4. Responsibility map — seven core files

| Interface + implementation | Main owned data | Main operations | Phase |
|---|---|---|---|
| `Bond.h` + `Bond.cpp` | face, coupon, maturity, frequency; computed cash flows | construct, validate, accessors, `regularCashFlows()` | 1 |
| `BondPricing.h` + `BondPricing.cpp` | solver settings/result values; no stateful pricer | `priceFromYTM()`, `solveYTM()`, `priceFromCurve()` | 1–3 |
| `BondRisk.h` + `BondRisk.cpp` | risk result record; no stateful calculator | `calculateBondRisk()`, `dv01FiniteDifference()`, `parallelCurveDV01()` | 2–4 |
| `YieldCurve.h` + `YieldCurve.cpp` | zero nodes, compounding, interpolation | `zeroRate()`, `discountFactor()`, `shiftedParallelBp()`, `shiftedTwoTenBp()` | 3 |
| `CurveTrade.h` + `CurveTrade.cpp` | 2Y/10Y legs with side and face notional | build DV01-neutral positions, `netParallelDV01()` | 4 |
| `ScenarioAnalysis.h` + `ScenarioAnalysis.cpp` | shock/result value types; no mutable engine | `evaluateScenario()` and full repricing | 5 |
| `MarketData.h` + `MarketData.cpp` | dated par/zero observations | CSV loading, date/tenor/rate checks | 6 |

`src/main.cpp` is an **entry point**, not an eighth analytics module: it should parse or demonstrate small experiments, call the core modules, and print or export results. Keep business and numerical logic out of `main.cpp`.

---

## 5. Testing map and numerical acceptance criteria

| Test file | Introduced | Tests to write |
|---|---|---|
| `tests/test_bond.cpp` | Phase 1 | `Bond(100,0.04,2,2)` has cash flows `(2,2,2,102)`; times `(0.5,1,1.5,2)`; invalid zero face/frequency rejected |
| `tests/test_pricing.cpp` | Phase 1; extended Phase 2 | Price at 5% YTM ≈ **$98.1190129**; price at 4% YTM = `$100`; zero-coupon price; higher yield implies lower price; bisection recovers known YTM; invalid brackets rejected |
| `tests/test_risk.cpp` | Phase 2 | Positive conventional DV01; analytic versus finite-difference risk; units per `$100` and `$1m` face; duration/convexity approximation error under small and large moves |
| `tests/test_curve.cpp` | Phase 3 | Known discount factors; compounding and interpolation cases; `priceFromCurve()` against manual sum; parallel-bump sensitivity; boundary handling |
| `tests/test_trades.cpp` | Phases 4–5 | Hedge sizing and signs; illustrative `$4m` 2Y / `$1m` 10Y example; midpoint first-order scenario P&L; full-repricing consistency; nonzero residual risk |
| `tests/test_market_data.cpp` | Phase 6, only when parser exists | Bad dates, gaps, missing tenors, rate-unit normalization, sorted/duplicate handling; par-vs-zero separation |

**Sample data to retain:** `data/sample/known_price_cases.csv` already contains the `$98.1190129` benchmark and other known cases. New `sample_zero_curve.csv` and `sample_2y10y_shocks.csv` should be tiny, fully documented, and suitable for independent hand checks.

**Important:** The midpoint's `$0`, `+$7,600`, `−$7,600` table is a **first-order DV01 arithmetic check** using assumed sensitivities, not the expected exact repricing output for every coupon-bond/curve combination.

### Suggested incremental CMake/CTest organization

**Do not replace the existing working CMake executable until the source files exist.** After Phase 1, refactor toward this structure (illustrative, not currently buildable):

```cmake
cmake_minimum_required(VERSION 3.16)
project(TreasuryCurveRisk LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(treasury_core
    src/Bond.cpp
    src/BondPricing.cpp
    # Append implemented .cpp files in later phases.
)
target_include_directories(treasury_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(treasury_curve_cli src/main.cpp)
target_link_libraries(treasury_curve_cli PRIVATE treasury_core)

enable_testing()
add_executable(test_bond tests/test_bond.cpp)
target_link_libraries(test_bond PRIVATE treasury_core)
add_test(NAME BondTest COMMAND test_bond)

# Register other test files as they are implemented.
```

Use CTest and a small test helper or assertions to start; no external test framework is required for the first milestone. Keep appropriate compiler warnings (`-Wall -Wextra -Wpedantic` on GCC/Clang or `/W4` on MSVC) when refactoring.

---

## 6. Phases 7–8 — keep historical simulation conditional

The FQE presentation asks for hypothetical historical positions, P&L and risk, but does **not** require a general-purpose trading engine. Keep advanced simulation files **out of the mandatory core** until a data/valuation feasibility check passes.

### Phase 7: two responsible delivery paths

**Path A — valid dated zero/discount curves available:** implement a **small synthetic historical experiment**. At entry define fixed-coupon bonds with actual dated payment schedules, initial price, two notionals and a DV01 hedge. As time advances, age the same bonds, handle coupons, mark remaining cash flows using each dated curve, apply explicitly assumed costs/funding, and reconcile cash and P&L. A simple prototype can initially live in a well-tested research driver; split files only when complexity warrants it.

**Conditional extracted modules (only if necessary):**

```text
include/treasury/DatedBond.h        [C] Dated coupons, maturity and remaining cash flows
include/treasury/Backtester.h       [C] Historical simulation controller and daily rows
include/treasury/CashLedger.h      [C] Signed coupon, funding, trading cash entries
src/DatedBond.cpp                  [C]
src/Backtester.cpp                 [C]
src/CashLedger.cpp                 [C]
tests/test_backtester.cpp          [C]
```

Possible data fields for a conditional `BacktestConfig` include `startDate`, `endDate`, `holdingDays`, `rebalancingRule`, `twoYearFaceAmountUsd`, `annualFundingRate`, `annualShortBorrowRate`, and `executionCostBp`. A conditional `BacktestRow` should capture `dateIso`, both positions, gross/net P&L, coupons, financing and transaction costs, and exposure. These names are **design notes**, not approved historical assumptions or core APIs.

**Path B — insufficient curve or accounting data:** deliver validated **deterministic** scenario analysis and descriptive 2Y/10Y yield-spread history. Do not report results as executable trades, strategy alpha or historical portfolio returns.

### Phase 8: results and report

- Rebuild from a clean checkout and rerun the documented tests.
- Output scenario tables and any defensible synthetic historical results. Optional `python/plot_results.py` can turn CSVs into figures.
- Compare first-order DV01 P&L to full repricing and describe approximation errors.
- State curve source and conventions, limitations, assumed financing and the difference between hypothetical valuations and tradeable prices.
- Ensure `README.md`, `PROJECT_STATUS.md` and the presentation reflect **actually completed** results.

---

## 7. Eight-phase dependency and delivery map

| Phase | Main files first created/extended | Exit gate |
|---|---|---|
| **1. Bond pricing fundamentals** | `Bond`, `BondPricing`, `test_bond`, `test_pricing` | Cash flows and benchmark prices independently checked |
| **2. Yield solving and sensitivities** | Extend `BondPricing`; add `BondRisk`, `test_risk` | YTM, duration, convexity and DV01 checks pass |
| **3. Treasury yield curves** | `YieldCurve`; extend `BondPricing`, `test_curve` | Term-dependent cash-flow discounts verified; start data-feasibility review |
| **4. 2Y–10Y portfolio construction** | `CurveTrade`; extend `BondRisk`, `test_trades` | Leg sizes and signed DV01 neutrality reproduced |
| **5. Scenario and stress analysis** | `ScenarioAnalysis`; extend `test_trades` | Parallel/steepening/flattening checks and repricing differences explained |
| **6. Historical Treasury data** | `MarketData`; optional parser tests and Python cleaning | Source/unit checks; historical simulation **go/no-go** |
| **7. Historical P&L (conditional)** | Optional `DatedBond`, `Backtester`, `CashLedger`, fixtures | Dated cash and cost ledger reconciled, or a clearly stated scenario-only result |
| **8. Final integration and report** | CMake/CTest, README, status, results, report | Clean reproducible outputs and presentation |

**Scheduling note:** Phase 2 spans Weeks 2–3, Phases 4 and 5 share Week 5. Do not attempt to finish all seven core modules in Week 1.

### Immediate Phase 1 implementation checklist

- [ ] Add `include/treasury/Bond.h` and `src/Bond.cpp`.
- [ ] Add `include/treasury/BondPricing.h` and `src/BondPricing.cpp`, implementing **only** `priceFromYTM()` initially.
- [ ] Write `tests/test_bond.cpp` for regular-period coupon cash flows.
- [ ] Write `tests/test_pricing.cpp` for discount, par and zero-coupon prices.
- [ ] Update `CMakeLists.txt` to compile the new sources and register the tests.
- [ ] Run the tests locally; record actual results and then update `PROJECT_STATUS.md`.

**Do not** create the more advanced backtesting classes just because they appear in this specification. The project's value comes from **correct, validated financial models**, not its file count.

---

## 8. Reference documents and financial conventions

- [README.md](../README.md) — scope, milestones, current research approach.
- [PROJECT_STATUS.md](../PROJECT_STATUS.md) — actual verified progress.
- [02_math_and_conventions.md](02_math_and_conventions.md) — formulas, units, price/yield convention.
- [03_trading_strategy.md](03_trading_strategy.md) — 2Y/10Y long/short construction and midpoint scenarios.
- [04_data_plan.md](04_data_plan.md) — Treasury data and par/zero distinctions.
- [05_backtest_methodology.md](05_backtest_methodology.md) — hypothetical instruments, cash/financing and limits.
- [06_validation_plan.md](06_validation_plan.md) — test cases and tolerances.
- [07_eight_week_roadmap.md](07_eight_week_roadmap.md) — schedule and acceptance gates.
- [09_decision_log.md](09_decision_log.md) — proposed versus confirmed modeling choices.

**Financial benchmark:** 2-year bond, $100 face, 4% annual coupon, 2 coupons/year, nominal annual YTM 5% compounded semiannually → theoretical price **approximately $98.1190129**. The example is a price formula benchmark, **not a completed software test**.