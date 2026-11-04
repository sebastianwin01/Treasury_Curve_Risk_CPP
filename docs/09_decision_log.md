# 09 — Decisions and deliberately deferred scope

## Confirmed project decisions (2026-11-04)

| Topic | Decision |
|---|---|
| Final project | Treasury Yield Curve Trading and Risk Analysis in C++ |
| Main research output | Validated **hypothetical instantaneous** P&L of DV01-balanced 2Y–10Y steepener/flattener |
| Language and build | C++20, CMake, CTest |
| Instrument style | $100-face reference, fixed-rate Treasury-style coupon bonds, regular semiannual payments |
| Rate input | One **nominal annual YTM per bond** (decimal), with semiannual compounding |
| Shock definition | Independently bump the 2Y and 10Y **bond YTMs** by specified basis points, with no time elapsed |
| Exposure | Long 2Y/short 10Y = steepener; match initial leg dollar DV01s |
| Reported P&L | First-order DV01 approximation and exact repricing of unchanged future coupons |
| Validation | Independent benchmark price, YTM recovery, sensitivity checks, trade-sign tests, CTest |
| Historical project work | **Optional descriptive 2Y–10Y par-yield spread only**; portfolio backtesting is future work |
| Explicit exclusions | No machine learning, HMM/Markov switching, predictive signal, live trading or portfolio financing simulation |
| Delivery time | Eight-week **planning budget**, about 5–7 hours/week |

## Pricing convention for first benchmark

$100 face, 4% annual coupon, 2-year maturity, two regular payments/year. Nominal 5% YTM compounded semiannually gives cash flows 2, 2, 2, 102 and `P ≈ $98.1190129`. Nominal 4% YTM gives `P = $100`.

These are **expected numbers**, not evidence of tests passing.

## Illustrative position convention (separate benchmark)

The FQE slides assume **2Y DV01 $190/bp per $1m** and **10Y DV01 $760/bp per $1m**, long **$4m 2Y** / short **$1m 10Y**. **$760/bp** matched magnitudes yield first-order $0 (parallel), +$7,600 (steepening), −$7,600 (flattening). These numbers do not define the true DV01 of the specific coupon bonds coded later.

## Exclusions and extensions

**Not part of required project:** settlement calendars, ex-coupon dates, accrued interest, clean prices, historical individual-security prices, financing, bond aging, short-security borrow, collateral, continuous rebalancing, repo, full curve bootstrapping and synthetic historical portfolio return attribution.

**After MVP, optional:** small zero-coupon discount-curve module; separate **descriptive** historical 2Y–10Y par-yield chart. Neither should be represented as evidence of executable returns.

**Future project:** dated synthetic bond backtesting, financing/coupon ledger and actual-security quotation workflows with independently verified data conventions. Do not create these modules until scope is explicitly reopened.

## Open *implementation*, not research-scope questions

- [ ] Name exact constructor/API signatures when implementing Phase 1.
- [ ] Choose bisection yield bracket and price/yield tolerances in Phase 2.
- [ ] Document finite-difference bump size and per-face DV01 scaling in Phase 3.
- [ ] Set a fixed face notional anchor for trade demonstrations in Phase 4.
- [ ] Set display precision and scenario error thresholds in Phase 5.
- [ ] Assign tests/code-review owners and a real FQE presentation deadline if known.

## Process for changing scope

Any proposal to add a historical backtester or full curve engine must document the reason, dataset, required tests and estimated work before altering the eight-week mandatory goals. Track completed code, not proposed features, in [../PROJECT_STATUS.md](../PROJECT_STATUS.md).
