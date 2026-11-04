# Treasury Yield Curve Trading and Risk Analysis in C++

**Baruch FQE | C++20 | Fixed income | Research and risk analysis**

## One research question

**How do immediate yield shocks affect a DV01-balanced 2-year versus 10-year Treasury-style steepener or flattener?**

This is a deliberately **small, testable eight-phase project**. It is **not** a live trading system, a prediction model, or a historical portfolio backtest.

## Required final result

Build a C++20 program that:

1. Generates cash flows for hypothetical 2Y and 10Y **fixed-coupon bonds** with regular semiannual payments.
2. Prices each bond from YTM and recovers YTM from price by bisection.
3. Computes Macaulay/modified duration, convexity and **DV01 ($/bp)**.
4. Sizes long-2Y/short-10Y (steepener) and reverse (flattener) positions so initial **signed parallel DV01** is near zero.
5. Evaluates **parallel +10/+10bp**, **steepening +5/+15bp**, and **flattening +15/+5bp** immediate YTM shocks.
6. Compares **first-order DV01 P&L** with **full bond repricing**, using automated tests and documented assumptions.

**Acceptance:** Build with CMake, run registered CTest tests, reproduce a benchmark bond price and print one scenario comparison table. **No downloaded market data is needed.**

## Scope: what we model

- Hypothetical **Treasury-style bonds**, not identified securities or executable prices.
- Prices quoted per **$100 face**, with face-notional scaling for trades.
- **Nominal annual YTM** represented as a decimal, **semiannual compounding**, regular coupon periods.
- A rate scenario independently bumps the **2Y bond YTM** and **10Y bond YTM**. Future cash flows remain fixed and **no time passes** between before/after valuations.
- Dollar P&L is **hypothetical immediate price sensitivity**, not historical trading profit.
- DV01 neutrality removes **first-order parallel exposure** under this bump convention, not convexity and other market risk.

**Not required:** Real Treasury settlement conventions, accrued interest, bond aging, borrowing, coupons paid over holding periods, transaction costs, rebalancing, financing ledgers, historical portfolio returns, zero-curve bootstrapping, machine learning, regime estimation or live orders.

**After the MVP only, optional:** a simple zero-discount curve exercise **or** a descriptive plot of historical 2Y–10Y Treasury par-yield spreads. Neither is required for completion.

## Repository — current tracked files

```text
Treasury_Curve_Risk_CPP/
├── README.md
├── PROJECT_STATUS.md
├── CMakeLists.txt
├── .gitignore
├── src/
│   └── main.cpp                       # currently a console scaffold
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

**Future code folders** `include/treasury/` and `tests/` will be created **with their first actual headers/tests**, not as empty placeholders. The target five-module design is in [technical architecture](docs/11_technical_architecture.md). The short work order is [Phase 1 — start here](docs/12_start_here.md).

## The eight achievable phases

| Phase | Deliverable | Evidence |
|---|---|---|
| 1 | Bond and cash flows; single-YTM pricing | Price/cash-flow tests |
| 2 | Bisection YTM solver | Root recovery and invalid-input tests |
| 3 | Duration, convexity and DV01 | Finite-difference checks |
| 4 | 2Y–10Y DV01-matched trade | Signed parallel exposure near zero |
| 5 | Parallel, steepener and flattener scenarios | Approximate vs full-repricing P&L |
| 6 | CTest and robustness | Clean build and test regression |
| 7 | Reproducible output and discussion | Tables, assumptions, optional descriptive chart |
| 8 | Final write-up and FQE presentation | Reviewer can reproduce results |

Details: [eight-week roadmap](docs/07_eight_week_roadmap.md), [validation plan](docs/06_validation_plan.md), and [current status](PROJECT_STATUS.md).

## Two independent benchmarks

**Bond pricing:** Face $100, coupon 4% annually, maturity 2 years, 2 coupons/year, nominal YTM 5% (semiannual discounting); cash flows `(2,2,2,102)`:

```text
P = 2/1.025 + 2/1.025^2 + 2/1.025^3 + 102/1.025^4
  ≈ $98.1190129
```

At 4% YTM, price equals $100.

**Illustrative DV01-balanced steepener (not calculated from those coupon bonds):** Assume the 2Y DV01 is $190 per bp per $1m face and the 10Y DV01 is $760 per bp per $1m face. Long $4m 2Y/short $1m 10Y has **$760/bp magnitude per leg**. First-order scenario P&L is $0 (parallel), +$7,600 (steepening) and −$7,600 (flattening). See [strategy](docs/03_trading_strategy.md).

Do **not** assume the actual pricer computes those illustrative DV01 inputs; the two checks are separate.

## Build the current scaffold

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/treasury_curve_cli
```

After Phase 1 tests are implemented and registered, also run:

```bash
ctest --test-dir build --output-on-failure
```

**Current status:** The repository contains documentation, benchmarks, CMake and a placeholder `src/main.cpp`; no completed financial pricing functions or passing financial tests have been established yet.

## Reference

Bruce Tuckman and Angel Serrat, *Fixed Income Securities: Tools for Today's Markets*, 4th ed. (2022), Chapters 1, 3 and 4 for coupon pricing, YTM and yield sensitivities. Additional references and reading tasks appear in the [roadmap](docs/07_eight_week_roadmap.md).
