# Project status and milestones

**Status as of project setup:** Scope and research question are defined. No independently validated C++ bond pricer, risk module, data-cleaning pipeline, historical P&L or tests have been established as completed. `main.cpp` currently compiles but contains only a placeholder.

## Checklist

- [x] Decide core research topic and objectives
- [x] Remove ML and Markov-switching from scope
- [x] Create documentation and C++20 project scaffold
- [ ] M1 — Understand cash flows and derive coupon-bond pricing
- [ ] M1 — Implement `price_from_ytm` and benchmark cases
- [ ] M2 — Implement YTM solver, with numerical convergence tests
- [ ] M2 — Implement duration, convexity and DV01, and cross-check by repricing
- [ ] M3 — Create a documented simple discount-curve representation
- [ ] M4 — Define a specific 2Y and 10Y bond selection / hedge convention
- [ ] M4 — Implement DV01-matched steepener/flatteners and shock scenarios
- [ ] M5 — Acquire and clean permissible historical Treasury data
- [ ] M6 — Design and implement position aging, coupon accounting, financing/cost assumptions, and P&L
- [ ] M7 — Validate full repricing against approximations; run sensitivity analyses
- [ ] M8 — Reproducible results, README, final report/presentation
- [ ] Optional — Non-ML descriptive regime comparison using prespecified definitions

## Definition of done

A milestone is complete only when formulas, unit conventions, C++ logic and independent checks are documented, and corresponding results can be reproduced from the repository.

**Never label an illustrative scenario as a historical backtest or evidence of alpha.**
