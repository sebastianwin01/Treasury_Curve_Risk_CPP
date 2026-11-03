# 07 — Eight-week implementation and learning roadmap

**Time budget:** approximately 5–7 hours/week, or 40–56 hours total, as described in the FQE midpoint context. This is a planned budget, not evidence of past work.

The time budget is aggressive. Protect validation and final reporting time; drop optional modules before compromising the core.

| Week | Priority | Research/learning task | Target deliverable |
|---|---|---|---|
| 1 | Essential | Read Tuckman Ch. 1; manually price coupon bonds | Correct cash flows, derivation and first `price_from_ytm` implementation |
| 2 | Essential | Study YTM (Ch. 3); root finding | Bisection YTM solver; par/discount/monotonicity tests |
| 3 | Essential | Study Ch. 4; risk derivatives | Modified/Macaulay duration, convexity, DV01; finite-difference checks |
| 4 | Essential | Study Ch. 2 & 5 as needed; define curve conventions | Simple discount curve and 2Y–10Y DV01-equivalent sizing |
| 5 | Essential | Program shock/repricing scenarios | Parallel, steepening, flattening; approximate vs. exact P&L |
| 6 | High | Source, clean and document historical observations | Reproducible input dataset and stated synthetic/actual instrument approach |
| 7 | High | Analyze historical path / costs / sensitivity | Honest simulated P&L and robustness/limitations, if data supports it |
| 8 | Essential | Fix bugs, validate, create final exhibits | README, charts, report and FQE presentation |

## Weekly rhythm

- **~1.5–2 hours:** Read theory and derive formulas.
- **~2.5–3 hours:** Code in C++20 (Python only where useful for data/plots).
- **~1–1.5 hours:** Manual tests, documentation and review.

## Priority if behind schedule

1. Preserve working bond pricer, YTM, DV01 and full repricing.
2. Preserve documented 2Y–10Y scenario analysis.
3. Simplify historical analysis honestly rather than claiming a production backtest.
4. Remove descriptive regime comparisons and nonessential dashboard features first.
5. Keep final verification and communication time.

## Review gates

- Gate A (end of week 2): bond pricing and YTM pass hand-calculated benchmarks.
- Gate B (end of week 3): risk sensitivities independently validated.
- Gate C (end of week 5): DV01-neutral trade sign and shocks verified.
- Gate D (end of week 7): historically based outputs, if any, have documented assumptions and checks.
- Gate E (end of week 8): every reported conclusion matches reproducible evidence.
