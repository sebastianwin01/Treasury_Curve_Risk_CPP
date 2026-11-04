# 07 — Eight-week implementation roadmap (non-ML)

**Planned budget:** approximately **5–7 hours per week (40–56 hours)**. These are estimates, not tracked completed hours. Protect testing, explanation and final-report time.

The eight phases below are logical deliverables; **Phases 4 and 5 share Week 5**, while Phase 2 spans Weeks 2–3. Do not interpret phase numbers as one phase per week.

| Phase | Week(s) | Primary learning and engineering work | Required deliverable / exit check |
|---|---|---|---|
| **1 — Bond pricing fundamentals** | 1 | Tuckman Chapter 1; cash-flow schedules, coupon/principal, present value, simple YTM valuation | `price_from_ytm`; manually reconciled $98.1190129 discount and $100 par benchmarks |
| **2 — Yield solving and risk sensitivities** | 2–3 | Ch. 3 YTM; bisection and convergence; Ch. 4 duration, convexity, DV01 | Solver and risk functions pass direct and finite-difference tests |
| **3 — Treasury yield curves** | 4 | Ch. 2; spot/par/YTM distinctions; discount factors, interpolation, per-coupon discounting | Small defined curve object, reproducible coupon-date curve-pricing cases |
| **4 — 2Y–10Y portfolio construction** | 5, first half | Long/short conventions, per-face DV01 and hedge-size calculation | Verified DV01-matched steepener/flattener legs |
| **5 — Scenario and stress analysis** | 5, second half | Parallel shifts, steepening/flattening, severe stress; DV01 approximation versus exact repricing | Checked scenario table and residual convexity risk |
| **6 — Historical Treasury data** | 6 | Source observation provenance, dated series, unit normalization, missing tenors, curve suitability | Reproducible clean input data and limitations report |
| **7 — Historical P&L and backtesting** | 7 | Synthetic bond initialization, aging, coupon flows, position rolls, financing and assumed costs | Synthetic P&L with cash ledger **only if data and conventions justify it** |
| **8 — Final integration and research report** | 8 | Test a clean checkout, reproduce outputs, inspect risks and caveats; prepare final FQE report | Reproducible runnable code, written research summary and presentation |

## Early dependency: data feasibility

**Begin source suitability checks during Phase 3.** U.S. Treasury constant-maturity par rates support 2Y/10Y slope analysis but are not sufficient by themselves to reprice every coupon payment using a zero curve. A Fed fitted zero curve may support **synthetic** bond valuation after verifying the rate conventions. No historical P&L claim should depend on treating fitted rates as live tradable quotes.

At the start of Phase 6 make a clear **go/no-go**:
- **Go:** a documented, internally consistent historical curve and bond-cash-flow convention exists; implement limited **synthetic historical** P&L.
- **No-go:** deliver full deterministic scenarios and an honest descriptive historical spread study instead; do **not** manufacture returns.

## Incremental checks, not end-of-project testing

- **Week 1:** Validate cash-flow dates/amounts, 5% discount-price case, 4% par case and monotonic price/yield relation.
- **Week 2:** Validate YTM convergence and input/bracketing failure paths.
- **Week 3:** Compare duration, convexity and DV01 with independent finite differences and full repricing.
- **Week 4:** Cross-check curve discounts and interpolation conventions on small examples.
- **Week 5:** Verify that long 2Y / short 10Y profits on first-order steepening, and matched parallel risk is near zero.
- **Week 6:** Reproduce transformed rows and identify missing dates, tenors and quotation units.
- **Week 7:** Reconcile position inventory, coupons, financing and gross/net P&L; stress alternative costs.
- **Week 8:** Clean build and documented rerun; report only measured evidence.

## Weekly working rhythm

- **1.5–2 hours:** Finance theory and hand calculations.
- **2.5–3 hours:** C++20 development; optional Python for non-ML data and plotting.
- **1–1.5 hours:** Automated tests, review and documentation.

These are flexible allocations within a demanding schedule.

## Priority when behind schedule

1. Preserve a correct bond pricer, YTM solver, duration and DV01, with tests.
2. Preserve DV01-equivalent 2Y–10Y sizing and deterministic full-repricing scenarios.
3. Scale down history to **descriptive** yields if sound synthetic valuation is infeasible.
4. Remove optional regime slices, advanced interpolation, extra instruments and plots before removing validation.
5. Reserve Week 8 for reproducibility and research communication.

## Delivery gates

| Gate | Deadline | Pass condition |
|---|---|---|
| A | End Week 2 | Cash-flow, price and YTM tests reproduce benchmarks |
| B | End Week 3 | Duration/DV01/convexity sensitivity checks pass |
| C | End Week 5 | Parallel and curve-shock trade P&L checks pass |
| D | End Week 7 | Any historical output is labeled synthetic and its assumptions are auditable |
| E | End Week 8 | Everything claimed in report is reproducible from code, documented inputs and commands |

Track progress in [PROJECT_STATUS.md](../PROJECT_STATUS.md), define exact simulation conventions in [05_backtest_methodology.md](05_backtest_methodology.md), and record open choices in [09_decision_log.md](09_decision_log.md).

