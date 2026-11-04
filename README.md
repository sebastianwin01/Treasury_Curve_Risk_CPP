# Treasury Yield Curve Trading and Risk Analysis in C++

**Baruch FQE fixed-income research project | Fall 2026 | C++20**

## Research question

**How do changes in the U.S. Treasury yield curve affect the risk and profit/loss (P&L) of DV01-balanced 2-year versus 10-year Treasury steepener and flattener positions?**

**Goal:** Develop a transparent, reproducible C++20 framework to price conventional fixed-coupon bonds, measure interest-rate risk, construct DV01-balanced 2Y–10Y trades, evaluate deterministic shocks, and, if defensible data are available, analyze **synthetic historical** position P&L.

This is an **educational quantitative research and paper-simulation project**, not a trading recommendation or execution system. The core objective is correct mathematics, traceable assumptions and numerical validation—not predicted returns.

## Scope

**Included:** U.S. nominal fixed-rate Treasury-style instruments, bond cash flows, YTM and curve-based valuation, duration/convexity/DV01, 2Y–10Y position sizing, parallel/nonparallel rate shocks, historical data preparation, synthetic valuation and research reporting. Python may support non-ML data preparation and visualization; C++20 is the core analytics implementation.

**Excluded:** Machine learning, forecasting models, Markov switching/HMMs, stochastic term-structure model calibration, live order execution, swaps/futures trading, credit products and an institutional-grade trading system. Simple **descriptive** comparisons (e.g., inverted versus upward-sloping curves) are optional only after the core works.

## Eight implementation phases

| Phase | Timing | Focus | Acceptance deliverable |
|---|---|---|---|
| **1. Bond pricing fundamentals** | Week 1 | Fixed coupons, principal repayment, present value and single-YTM pricing | Independently checked bond cash flows and `price_from_ytm` |
| **2. Yield solving and risk sensitivities** | Weeks 2–3 | YTM bisection, Macaulay/modified duration, convexity and DV01 | Validated numerical solver and risk functions |
| **3. Treasury yield curves** | Week 4 | Spot/par/YTM distinctions, discount factors and documented interpolation | Small `YieldCurve` representation and curve-priced bond |
| **4. 2Y–10Y portfolio construction** | Week 5 | Long/short exposure, DV01 matching and explicit hedge units | Position-sizing rules with checked exposure |
| **5. Scenario and stress analysis** | Week 5 | Parallel, steepener, flattener and larger shocks; full repricing | Deterministic scenario P&L versus DV01/convexity approximations |
| **6. Historical Treasury data** | Week 6 | Data source, normalized rates, missing values and data provenance | Reproducible curated dataset, with valuation limitations |
| **7. Historical P&L and backtesting** | Week 7 | Synthetic bond aging, cash flows, funding/cost assumptions and sensitivity | Clearly labeled synthetic historical results **if feasible** |
| **8. Final integration and research report** | Week 8 | Regression checks, charts, limitations and presentation | Reproducible documented results and FQE report |

The phases are **implementation stages**, not a claim that work has already been completed. Investigate historical data suitability as early as Phase 3; do not postpone feasibility checks until Week 6. See [the roadmap](docs/07_eight_week_roadmap.md) and [current status](PROJECT_STATUS.md).

## Valuation approaches and what results mean

**Single-yield model for the first bond exercise:**

```text
P(y) = SUM_{k=1}^{N} CF_k / (1 + y/m)^k
```

Here `y` is nominal annual YTM (decimal), `m` coupons per year, and `CF_k` is the cash flow at regular coupon period `k`.

**Curve-based valuation for later scenarios:**

```text
P_curve = SUM_{k=1}^{N} CF_k * D(t_k)
```

`D(t_k)` is a cash-flow-date discount factor derived from a correctly specified term structure. **Treasury constant-maturity par yields are not zero rates, discount factors, bond-specific YTMs or executable bond prices.**

**Historical target:** synthetic bonds with specified inception coupons and maturity dates, repriced through time using documented curve inputs. Any P&L from fitted or synthesized yields must be labeled **synthetic / simulated**, never realized market performance. If appropriate curve inputs or conventions cannot be established, deliver rigorous scenario research instead of an unsupported historical backtest. See [backtest methodology](docs/05_backtest_methodology.md).

## Repo structure

```text
Treasury_Curve_Risk_CPP/
├── README.md
├── PROJECT_STATUS.md
├── CMakeLists.txt
├── .gitignore
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
│   └── 10_final_report_outline.md
├── include/               # Interfaces: future work
├── src/main.cpp            # Console scaffold only, as of current review
├── tests/                  # Automated tests: planned
├── data/
│   ├── sample/known_price_cases.csv
│   ├── raw/
│   └── processed/
├── python/                 # Optional non-ML data utilities
└── results/                # Outputs, not yet produced
```

## First verified target: Milestone 1

Hypothetical bond: face **$100**, annual coupon **4%**, maturity **2 years**, semiannual coupons, nominal annual YTM **5%** with semiannual compounding.

- Four cash flows: **$2, $2, $2, $102**.
- Price at 5% YTM: approximately **$98.1190129**.
- Price at 4% YTM: **$100**.
- Higher YTM must lower the price for these positive fixed cash flows.

The target numbers are independent benchmarks; **the C++ function has not yet been implemented or tested in the committed repository**. Start with a simple function and tests, not a large class hierarchy. Test plan: [docs/06_validation_plan.md](docs/06_validation_plan.md).

## Build the current scaffold

Open the root directory in CLion or run:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/treasury_curve_cli
```

These are build commands, **not evidence that validation tests have passed**. At the time of this documentation update, the main C++ executable is still a placeholder, and no pricing engine, historical data pipeline or backtest is committed.

## Development method

Financial intuition → mathematical derivation → independently worked example → algorithm → C++20 implementation → automated tests → financial interpretation → engineering improvements.

Every completed phase must have its conventions, code and independent validation documented. Keep **confirmed decisions**, **proposed defaults** and **unresolved choices** separate in [docs/09_decision_log.md](docs/09_decision_log.md).

## References

- Bruce Tuckman and Angel Serrat, *Fixed Income Securities: Tools for Today's Markets*, 4th ed. (2022), especially Chapters 1–6.
- [Federal Reserve — Treasury Nominal Yield Curve](https://www.federalreserve.gov/data/nominal-yield-curve.htm) (fitted curve inputs; verify series and conventions).
- [U.S. Treasury — Interest Rate Statistics](https://home.treasury.gov/policy-issues/financing-the-government/interest-rate-statistics) (par-yield history useful for descriptive analysis; not executable security prices).

