# Treasury Yield Curve Trading and Risk Analysis in C++

**Baruch FQE fixed-income research project | Fall 2026 | C++20**

## Project at a glance

**Research question:** How do yield-curve changes affect the risk and profit/loss (P&L) of dollar-duration (DV01)-balanced **2-year vs. 10-year U.S. Treasury steepener and flattener trades**?

**Goal:** Build and validate a reproducible quantitative research framework that prices standard fixed-rate bonds, computes interest-rate risk, constructs 2Y–10Y relative-value trades, and evaluates their scenario and historical P&L.

**Scope decision: NO machine learning.** No ML forecasting, PCA-based predictive models, Hidden Markov models, Markov-switching estimation, Gauss+ calibration, or automated trading is required. A **transparent descriptive regime comparison** (e.g., inverted vs. upward-sloping curves) is an optional stretch feature, not an ML model.

The baseline is **research and paper simulation**, not live execution or a promise of profit.

## What the finished project should do

1. Generate fixed-coupon bond cash flows and calculate cleanly specified present values.
2. Price from a single yield to maturity (YTM) and numerically solve YTM from price.
3. Compute modified/Macaulay duration, convexity, and DV01 with stated units and conventions.
4. Represent a simple curve of discount factors (after proper convention checks), value cash flows using a curve, and distinguish par, spot, and YTM rates.
5. Build long/short 2Y–10Y trades with matched DV01 magnitudes; stress them against parallel shifts, steepening, and flattening.
6. Conduct reproducible historical P&L analysis under explicit assumptions about instrument selection, aging, coupon cash flows, rebalancing, financing and costs.
7. Report risks, errors, scenarios, and limitations honestly.

## Scope boundaries

**In scope:** U.S. Treasury nominal fixed-rate bonds; 2Y and 10Y; C++20 analytics and tests; Python only for optional data preparation, charts, and descriptive statistics; historical yield data; simple deterministic strategy rules and scenario analysis.

**Out of scope:** ML, predictive alpha claims, live brokerage integration, swaps, futures execution, complex stochastic term-structure models, options, credit risk, or an institutional execution system.

## Repository guide

```text
Treasury_Curve_Risk_CPP/
├── README.md                       <- start here
├── PROJECT_STATUS.md               <- milestones and current status
├── CMakeLists.txt                   <- CLion / CMake scaffold (C++20)
├── .gitignore
├── docs/
│   ├── 01_project_charter.md        <- objectives, scope, success criteria
│   ├── 02_math_and_conventions.md   <- pricing and risk formulas
│   ├── 03_trading_strategy.md       <- 2Y–10Y hedging and examples
│   ├── 04_data_plan.md              <- sourcing, fields, caveats
│   ├── 05_backtest_methodology.md   <- research design, biases, costs
│   ├── 06_validation_plan.md        <- acceptance tests
│   ├── 07_eight_week_roadmap.md     <- 8-week effort plan
│   ├── 08_reading_map.md            <- Tuckman chapter guide
│   ├── 09_decision_log.md           <- assumptions to lock down
│   └── 10_final_report_outline.md  <- presentation / report structure
├── include/                        <- future C++ interfaces (not implemented)
├── src/main.cpp                    <- buildable console placeholder
├── tests/                          <- future executable unit tests
├── data/
│   ├── sample/known_price_cases.csv
│   ├── raw/README.md
│   └── processed/README.md
├── python/README.md                <- optional NON-ML helper scripts
└── results/README.md               <- generated charts / tables / logs
```

## First task — Milestone 1

Using a hypothetical 2-year fixed coupon bond with a $100 face amount, a 4% annual coupon, two coupons per year, and a nominal annual YTM of 5% compounded semiannually:

- Cash flows per half-year are **$2, $2, $2, $102**.
- Price at **5% YTM: ~$98.1190129**.
- Price at **4% YTM: exactly $100** in the simplified regular-period setup.
- Increasing YTM should lower price for this positive-cash-flow bond.

First manually derive the formula, then independently implement the `price_from_ytm` function and tests. Avoid adding an elaborate bond class before the simple functions are validated.

## Run the C++20 project scaffold

Open this root folder in CLion, or from a terminal:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/treasury_curve_cli
```

**Important:** `src/main.cpp` is intentionally only a starter placeholder. The project has **not** yet implemented pricing, tests, a data pipeline, or historical trading results. Add these milestones incrementally; do not present the scaffold as a finished quant engine.

## Working standard

For every new module: intuition → mathematical derivation → hand-checked example → algorithm design → C++20 implementation → independently validated tests → financial interpretation → refactor.

## Primary references

- Bruce Tuckman and Angel Serrat, *Fixed Income Securities: Tools for Today's Markets*, 4th ed. (2022), especially Chapters 1–6.
- Federal Reserve: [Treasury Nominal Yield Curve](https://www.federalreserve.gov/data/nominal-yield-curve.htm) (a fitted curve; instrument/pricing conventions must be documented).
- U.S. Treasury: [Interest rate statistics](https://home.treasury.gov/policy-issues/financing-the-government/interest-rate-statistics) (published par-yield series are not zero curves or executable bond quotes).

Read [PROJECT_STATUS.md](PROJECT_STATUS.md) and [docs/07_eight_week_roadmap.md](docs/07_eight_week_roadmap.md) next.
