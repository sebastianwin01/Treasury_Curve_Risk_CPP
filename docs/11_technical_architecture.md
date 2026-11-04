# 11 — Technical architecture: five-module C++20 MVP

**Status: design, not implemented code.** The current repository contains a working scaffold, but no finished finance modules or registered finance tests. Create modules **only when needed**.

## Tracked repository after cleanup

```text
Treasury_Curve_Risk_CPP/
├── .gitignore
├── CMakeLists.txt
├── README.md
├── PROJECT_STATUS.md
├── src/
│   └── main.cpp                       # existing console placeholder
├── data/
│   └── sample/
│       ├── known_price_cases.csv
│       └── illustrative_trade_scenarios.csv
└── docs/
    ├── 02_math_and_conventions.md
    ├── 03_trading_strategy.md
    ├── 06_validation_plan.md
    ├── 07_eight_week_roadmap.md
    ├── 11_technical_architecture.md
    └── 12_start_here.md
```

## Future source files (create phase by phase, not now)

```text
include/treasury/
├── Bond.h                  [Phase 1]
├── BondPricing.h           [Phases 1–2]
├── BondRisk.h              [Phase 3]
├── CurveTrade.h            [Phase 4]
└── ScenarioAnalysis.h      [Phase 5]

src/
├── main.cpp                [already exists]
├── Bond.cpp
├── BondPricing.cpp
├── BondRisk.cpp
├── CurveTrade.cpp
└── ScenarioAnalysis.cpp

tests/
├── test_bond.cpp
├── test_pricing.cpp
├── test_risk.cpp
├── test_trade.cpp
└── test_scenarios.cpp
```

**No optional folders, empty headers, C++ files or empty tests need to be committed before implementation.**

## 1. `Bond.h` and `Bond.cpp` — regular cash flows

```cpp
#pragma once
#include <vector>

namespace treasury {

struct CashFlow {
    double timeYears{};
    double amountUsd{};
};

class Bond {
    double faceValueUsd_;
    double annualCouponRate_;
    double maturityYears_;
    int couponsPerYear_;
public:
    Bond(double faceValueUsd, double annualCouponRate,
         double maturityYears, int couponsPerYear);

    double faceValueUsd() const noexcept;
    double annualCouponRate() const noexcept;
    double maturityYears() const noexcept;
    int couponsPerYear() const noexcept;
    std::vector<CashFlow> cashFlows() const;
};

} // namespace treasury
```

The constructor must validate positive face/maturity/frequency, nonnegative coupon and a positive integer number of regular coupon periods `N=m*T`. Cash flows: `N` payments, coupon `F*c/m` each, final payment plus `F`. The test asserts `(2,2,2,102)` on the example bond. No manual owning pointers needed.

## 2. `BondPricing.h` and `BondPricing.cpp` — YTM price and solver

```cpp
#pragma once
#include "treasury/Bond.h"

namespace treasury {

double priceFromYTM(const Bond& bond, double annualYtmDecimal);

double solveYTMFromPrice(
    const Bond& bond, double targetPriceUsd,
    double lowerYieldDecimal, double upperYieldDecimal,
    double tolerance=1e-12, int maxIterations=200);

} // namespace treasury
```

- `priceFromYTM` discounts the fixed cash flows at `(1+y/m)^k`; reject non-finite/invalid compounding bases.
- `solveYTMFromPrice` uses bisection; require a valid bracket and explicit failure on invalid inputs/nonconvergence.
- Tests: benchmark price **$98.1190129** at 5% YTM, par at 4%, zero coupon, yield monotonicity and recovered YTM.

## 3. `BondRisk.h` and `BondRisk.cpp` — duration/convexity/DV01

```cpp
#pragma once
#include "treasury/Bond.h"

namespace treasury {

struct BondRiskMetrics {
    double macaulayDurationYears{};
    double modifiedDurationYears{};
    double convexityYearsSquared{};
    double dv01UsdPerBp{}; // per bond.faceValueUsd() quoted amount
};

BondRiskMetrics calculateRisk(
    const Bond& bond, double annualYtmDecimal);

double dv01FiniteDifference(
    const Bond& bond, double annualYtmDecimal,
    double bumpDecimal=0.0001);

} // namespace treasury
```

Use yield derivatives from [math notes](02_math_and_conventions.md), verify with symmetric differences and correct `USD/bp` units. Report results per the bond's reference face. Do not conflate these calculated DV01s with the slide's assumed $190/$760 figures.

## 4. `CurveTrade.h` and `CurveTrade.cpp` — two signed legs

```cpp
#pragma once
#include "treasury/Bond.h"

namespace treasury {

struct TradeLeg {
    Bond bond;
    double startingYtmDecimal;
    double signedFaceNotionalUsd; // >0 long, <0 short
};

struct CurveTrade {
    TradeLeg twoYear;
    TradeLeg tenYear;
};

CurveTrade makeDv01Steepener(
    const Bond& bond2y, double ytm2y,
    const Bond& bond10y, double ytm10y,
    double twoYearAnchorFaceUsd);

CurveTrade makeDv01Flattener(
    const Bond& bond2y, double ytm2y,
    const Bond& bond10y, double ytm10y,
    double twoYearAnchorFaceUsd);

double signedParallelDv01UsdPerBp(const CurveTrade& trade);

} // namespace treasury
```

Compute DV01 **per unit of face**, multiply by signed face notionals and size the 10Y notional to match the 2Y dollar DV01. A steepener is 2Y long/10Y short; a flattener reverses both. Unit tests must check orientation and parallel hedge accuracy.

## 5. `ScenarioAnalysis.h` and `ScenarioAnalysis.cpp` — immediate shocks

```cpp
#pragma once
#include "treasury/CurveTrade.h"

namespace treasury {

struct RateScenario {
    const char* name;
    double delta2yBp;
    double delta10yBp;
};

struct ScenarioResult {
    double firstOrderPnlUsd;
    double fullRepricingPnlUsd;
    double approximationErrorUsd;
};

ScenarioResult evaluateScenario(
    const CurveTrade& trade, const RateScenario& scenario);

} // namespace treasury
```

- First-order P&L = negative of each **signed face-scaled DV01** times its yield change in bp.
- Full repricing P&L = sum of signed face-scaled differences in `priceFromYTM(new) - priceFromYTM(old)`.
- Error = full repricing minus first-order estimate.
- Test parallel +10/+10bp, steepening +5/+15bp and flattening +15/+5bp.
- *No time passes*; this is **not** an observed financial backtest.

## 6. `main.cpp` — demonstration

Eventually construct the 2Y and 10Y bonds, print their prices and risk, build the matched long/short trade and run the three scenarios. Report 2Y/10Y yield bumps, first-order and full-repricing P&L, difference, position signs and face notionals. Print a sentence explicitly labeling the table **hypothetical**.

## 7. CMake and test approach

Initially the committed CMake file should continue building `src/main.cpp`. **As files are created**, add them to the executable or a reusable `treasury_core` static library. Enable CTest and add test targets only after their source files exist.

Final target outline (not directly applicable until sources exist):

```cmake
add_library(treasury_core
    src/Bond.cpp
    src/BondPricing.cpp
    src/BondRisk.cpp
    src/CurveTrade.cpp
    src/ScenarioAnalysis.cpp)
target_include_directories(treasury_core PUBLIC include)
target_link_libraries(treasury_curve_cli PRIVATE treasury_core)
enable_testing()
# Register each test executable as it is implemented.
```

Use C++20, warning flags, `std::vector`, `const` accessors, meaningful exceptions and independent numerical tests. The required source dependencies form a simple chain:

```text
Bond → BondPricing → BondRisk → CurveTrade → ScenarioAnalysis → main
```

## 8. Completion gates

| Phase | Implementation | Independent validation |
|---|---|---|
| 1 | Bond + YTM price | Coupons, discount/par pricing |
| 2 | Bisection solver | Recover known YTM; invalid bracket |
| 3 | Risk | Derivative/finite-difference comparison |
| 4 | Trade sizing | Equal-and-opposite signed DV01 |
| 5 | Scenarios | Approximate and full repricing P&L |
| 6 | Robustness | Clean build, CTest and errors |
| 7 | Report draft | Reproducible scenario table |
| 8 | Final | FQE presentation and limitations |

No historical bond backtester, dated cash ledger, financing engine, zero-curve module or market data loader is part of this required architecture.

**Start with:** [12_start_here.md](12_start_here.md). Check formulas: [02_math_and_conventions.md](02_math_and_conventions.md). Verify each phase: [06_validation_plan.md](06_validation_plan.md).
