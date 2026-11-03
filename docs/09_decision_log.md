# 09 — Decisions and unresolved questions

## Confirmed scope decisions

| Decision | Choice |
|---|---|
| Research focus | 2-year vs. 10-year U.S. Treasury curve risk and P&L |
| Primary implementation | C++20 |
| Support tools | Python for data handling, plots and descriptive statistics only |
| ML / Markov switching | **Discarded / not in project scope** |
| Project emphasis | Correct analytics, realistic assumptions, validation, transparent reporting |
| First milestone | $100 face, 4% coupon, 2-year, 5% YTM bond pricer |
| Initial timeline | Eight-week proposed work plan, ~5–7 hours/week |

## Open research and engineering choices

- [ ] Will we use hypothetical synthetic bonds, actual identified issues, or both?
- [ ] Historical data choice: Treasury par yields for descriptive spread study, Fed fitted zero/discount data for model valuation, or additional actual bond prices?
- [ ] Day-count, settlement, payment-date and clean/dirty conventions.
- [ ] Exact parallel shock and curve interpolation definition for DV01.
- [ ] Whether hedges are rolled/rebalanced, and when.
- [ ] Holding-period definition and financing model.
- [ ] Transaction costs, slippage, short borrowing and eligible position sizes.
- [ ] Whether there are any actual trading signals; baseline may be purely scenario/risk analysis.
- [ ] Which project collaborator owns which components and review milestones.
- [ ] Presentation date and final repository link.

## Decision template

**Date:** YYYY-MM-DD  
**Decision:** ...  
**Options considered:** ...  
**Rationale:** ...  
**Impact on formulas/data/tests:** ...  
**Owner/reviewer:** ...

Keep unresolved assumptions explicit rather than burying them in source code defaults.
