# Project status — achievable FQE MVP

**Review date:** 2026-11-04  
**Current evidence:** Project specification, C++20 CMake scaffold, benchmark CSV and documentation are in the repository. The actual bond-pricing engine, portfolio analysis and automated financial tests **have not been implemented or validated**.

## What counts as complete?

A phase is complete **only when code is committed, can be built and has reproducible tests/results**. A documented formula or hand-computed expected value is not a passed software test.

## Setup

- [x] Fix research question: 2Y–10Y DV01-neutral shock P&L.
- [x] Confirm C++20, no ML, no live trading.
- [x] Reduce required scope to immediate YTM-shock scenarios.
- [x] Provide an eight-phase learning/implementation plan.
- [x] Establish benchmark input files.
- [ ] Deliver first working bond-pricing implementation and automated test.

## Eight phase gates

| Phase | Current status | Must be demonstrated |
|---|---|---|
| 1. Bond and pricing | Not started | Correct regular coupons; $98.1190129 price, $100 par |
| 2. YTM solver | Not started | Recover original yield; invalid bracket fails clearly |
| 3. Risk metrics | Not started | Duration, convexity and DV01 independently checked |
| 4. Balanced trade | Not started | 2Y long/10Y short with near-zero signed parallel DV01 |
| 5. Stress scenarios | Not started | Three signed shock examples and exact repricing |
| 6. Test integration | Not started | Registered passing CTest cases; clean build |
| 7. Results and interpretation | Not started | Reproducible table, units and limitations; history optional |
| 8. Final report | Not started | Documented reproducible FQE presentation/report |

## Mandatory MVP output

1. Reference bond price test using `data/sample/known_price_cases.csv`.
2. Unit tests for YTM, duration, DV01, hedge weights and P&L signs.
3. One table comparing parallel +10/+10bp, steepening +5/+15bp and flattening +15/+5bp scenarios.
4. Both **DV01 first-order P&L** and **exact two-YTM repricing**; no claim that either is realized P&L.
5. Console command and CMake/CTest instructions in the README.

The illustrative `$190 / $760 per $1m` slide values are **separate reference assumptions**; do not hard-code them as if calculated by the bond pricer.

## Deferred — NOT blockers for completion

- [ ] Zero-curve coupon-date pricing (optional).
- [ ] Observed Treasury 2Y–10Y par-yield spread chart (optional).
- [ ] Full synthetic historical portfolio backtest, coupon calendar, financing and short-borrow ledger (future project).
- [ ] Markov switching, ML or predictive models (**excluded**).

## Next smallest task

Implement `Bond.h`/`Bond.cpp` (cash flows) and `BondPricing.h`/`BondPricing.cpp` (price from YTM); add a test file and a CTest target. **Do not mark Phase 1 complete until tests pass.** See [Phase 1 work order](docs/12_start_here.md).

Every future completed phase should record the **commit SHA**, test command and numerical tolerance.
