# Treasury Yield Curve Trading and Risk Analysis in C++

**Baruch FQE | C++20 | Fixed income | Non-ML**

## Research question

**How do interest-rate shocks affect the risk and theoretical P&L of a DV01-balanced 2-year versus 10-year Treasury-style steepener or flattener?**

## Minimum viable project (MVP)

Build **one small, reproducible C++20 console application** that:

1. Creates two hypothetical **fixed-coupon** bonds (2 years and 10 years) with regular semiannual payments.
2. Calculates their cash flows, prices from nominal annual YTM, and YTM from price.
3. Computes Macaulay and modified duration, convexity, and **dollar DV01 per 1bp**.
4. Constructs **DV01-balanced** long-2Y/short-10Y and reverse positions.
5. Tests **parallel**, **steepening** and **flattening** immediate yield shocks using both first-order P&L and full repricing.
6. Runs C++ automated tests and prints/reproduces a small results table.

**Definition of done:** a fresh clone builds with CMake, automated tests pass, reference bond pricing is reproduced, trade exposure signs are correct, and one clearly labeled hypothetical scenario table is produced. A historical trading backtest is **not required**.

### Explicit simplifying assumptions

- Hypothetical **Treasury-style** bonds, not actual identified securities or executable quotes.
- Valuation per **$100 face**, scaled by face notional when sizing positions.
- Fixed coupons, **regular semiannual periods**, and simple nominal annual YTM; no settlement calendars, accrued-interest/clean-price handling, reinvestment, rebalancing, or financing in the immediate-shock scenarios.
- At scenario time 0, shift the **2Y bond's YTM** by `Δy2` and the **10Y bond's YTM** by `Δy10`; reprice unchanged cash flows. This is a **two-YTM scenario model**, **not** a fitted full zero curve or a market-executable backtest.
- Long 2Y/short 10Y is a steepener. Match initial absolute leg DV01s to reduce **first-order parallel yield sensitivity**, not all risks.
- No machine learning, Markov switching, signal forecasting or live orders.

## Scope ladder

| Tier | Scope | Needed for FQE final? |
|---|---|---|
| **MVP — required** | Bond pricing, YTM, risk, DV01-matched trade, immediate shocks, CTest, scenario table | **Yes** |
| **Stretch A** | Basic zero-rate discount curve and coupon-date pricing, with explicit rate conventions | No |
| **Stretch B** | Descriptive historical 2Y–10Y spread from observed par-yield series; no simulated bond returns | No |
| **Future research** | Actual dated synthetic instruments, accrued interest, coupons, financing, rebalancing and historical portfolio P&L | **No** |

The MVP requires **five C++ modules**: `Bond`, `BondPricing`, `BondRisk`, `CurveTrade` and `ScenarioAnalysis`. `YieldCurve` and `MarketData` are **optional modules** and will only be created after the MVP passes.

## Eight concrete work phases

| Phase | Work | Exit evidence |
|---|---|---|
| **1** | Bond, regular cash flows and pricing | $100 / 4% / 2Y / 5% benchmark and par case pass |
| **2** | YTM bisection | Solver recovers 5% and handles bad brackets |
| **3** | Duration, convexity and DV01 | Independent finite-difference checks pass |
| **4** | DV01-neutral 2Y–10Y trade sizing | Equal-and-opposite signed parallel DV01 |
| **5** | Three immediate shock scenarios | First-order and exact-repricing P&L table |
| **6** | Automated regression and edge cases | CTest suite and clean CMake build |
| **7** | Reproduce results; optional descriptive historical spread | Documented output with units and assumptions |
| **8** | Final readme, charts, report and FQE presentation | Reviewer can clone, run, explain and reproduce |

See [the detailed schedule](docs/07_eight_week_roadmap.md), [technical architecture](docs/11_technical_architecture.md), and [Phase 1 instructions](docs/12_start_here.md).

## Repository status

**Documentation/planning is present.** Financial classes and tests described above are a **target**, not already implemented. The committed `src/main.cpp` remains a starter console program. Check [PROJECT_STATUS.md](PROJECT_STATUS.md) for verified work, not intended progress.

## Planned MVP source layout

```text
include/treasury/
    Bond.h                 [Phase 1]
    BondPricing.h          [Phases 1-2]
    BondRisk.h             [Phase 3]
    CurveTrade.h           [Phase 4]
    ScenarioAnalysis.h     [Phase 5]
src/
    Bond.cpp
    BondPricing.cpp
    BondRisk.cpp
    CurveTrade.cpp
    ScenarioAnalysis.cpp
    main.cpp               [exists; placeholder]
tests/
    test_bond.cpp
    test_pricing.cpp
    test_risk.cpp
    test_trade.cpp
    test_scenarios.cpp
data/sample/
    known_price_cases.csv  [exists]
    illustrative_trade_scenarios.csv [reference only]
```

**Create implementation files only as their phase starts.** The full planned tree and APIs are in [docs/11_technical_architecture.md](docs/11_technical_architecture.md).

## Independent numerical benchmarks

A hypothetical **$100 face, 4% annual coupon, 2-year, semiannual** bond has cash flows `[2, 2, 2, 102]`. At nominal annual 5% YTM compounded semiannually:

```text
P = 2/1.025 + 2/1.025^2 + 2/1.025^3 + 102/1.025^4
  ≈ $98.1190129
```

At 4% YTM it prices at **$100**. For validation, a separate illustrative trade from the FQE midpoint slides assumes **2Y DV01 $190/bp per $1m**, **10Y DV01 $760/bp per $1m**, long **$4m 2Y** and short **$1m 10Y**, so each leg has **$760/bp** exposure. Under +5bp 2Y / +15bp 10Y, first-order P&L is **+$7,600**. **These assumed DV01s are not expected to equal the DV01s computed for the example coupon bonds.**

## Build the current console scaffold

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/treasury_curve_cli
```

Once tests are written and registered in CMake, also run:

```bash
ctest --test-dir build --output-on-failure
```

**CTest does not currently contain financial tests; this command is a future acceptance step.**

## References and caveats

- Bruce Tuckman and Angel Serrat, *Fixed Income Securities: Tools for Today's Markets*, 4th ed. (2022), especially Chapters 1, 3 and 4.
- [U.S. Treasury rate statistics](https://home.treasury.gov/policy-issues/financing-the-government/interest-rate-statistics) — useful for optional descriptive 2Y–10Y par-yield history; **not executable bond prices**.
- [Federal Reserve Treasury Nominal Yield Curve](https://www.federalreserve.gov/data/nominal-yield-curve.htm) — possible future curve-data extension requiring verified conventions.

All scenario profits are **hypothetical immediate repricing effects**, not actual or historical returns.
