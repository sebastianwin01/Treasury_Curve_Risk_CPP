# Project status and validation gates

**Repository assessment:** Documentation and a C++20 console scaffold are present. At the latest reviewed `main` commit, `src/main.cpp` is a placeholder. No bond-pricing implementation, automated test executable, historical data-processing pipeline, instrument-level valuation or trading backtest has been verified in the repository.

**Status semantics:**
- **Complete** = deliverable is committed and independently reproduced/validated.
- **In progress** = implementation or evidence exists, but validation is incomplete.
- **Not started** = planned in documentation only.
- An independently computed **benchmark value does not count as a passed C++ test**.

## Confirmed setup tasks

- [x] Define the 2Y–10Y fixed-income research question and scope.
- [x] Exclude machine learning and Markov-switching models.
- [x] Commit the C++20 CMake console scaffold and documentation structure.
- [x] Specify benchmark expectations and a validation plan.
- [ ] Implement and run an automated C++ financial test.

## Implementation-phase tracking

| Phase | Status | Evidence required to mark complete |
|---|---|---|
| **1. Bond pricing fundamentals** | Not started | Cash-flow generator and `price_from_ytm` implemented, benchmark/par/yield-monotonicity tests passing |
| **2. Yield solving and risk sensitivities** | Not started | Bisection solver with convergence checks; duration, convexity and DV01 cross-checked against full repricing |
| **3. Treasury yield curves** | Not started | Defined curve inputs and interpolation; discount-factor conversion verified and coupon-date curve pricing tested |
| **4. 2Y–10Y portfolio construction** | Not started | Documented instrument and size units, DV01-matched long/short legs and parallel exposure checks |
| **5. Scenario and stress analysis** | Not started | Parallel, steepening, flattening and large-move scenarios, analytic/finite-difference versus full P&L results |
| **6. Historical Treasury data** | Not started | Source/provenance, repeatable download/transform, normalized rate conventions, gaps and sample-period report |
| **7. Historical P&L and backtesting** | Not started | Synthetic instruments/coupons/aging, daily cash ledger, funding and costs, reported errors/limits; only if supportable |
| **8. Final integration and research report** | Not started | Clean build, tests, reproducible figures/tables, research conclusions, limitations and presentation |

**Optional only after core validation:** prespecified, retrospective non-ML descriptive comparisons of curve regimes. This is not part of the required eight phases.

## Review gates

- [ ] **Gate A — end Week 2:** prices, par cases, cash flows and YTM solver reconcile against independent calculations.
- [ ] **Gate B — end Week 3:** analytic/finite-difference interest-rate risk measures and unit conventions validated.
- [ ] **Gate C — end Week 5:** 2Y–10Y sizing, exposure signs and rate-shock scenario P&L independently checked.
- [ ] **Gate D — end Week 7:** if historical results exist, their instrument definitions, coupon ledger and assumptions are documented and reproducible. Otherwise publish **scenario-only** findings honestly.
- [ ] **Gate E — end Week 8:** final submitted claims match reproducible code and data; no unlabeled synthetic or hypothetical returns.

## Next engineering actions

1. Create `include/BondPricing.h` and `src/BondPricing.cpp` with a small, well-defined pricing API.
2. Implement regular fixed-coupon cash-flow generation and YTM pricing.
3. Add `tests/test_bond_pricing.cpp`, hook it into CMake/CTest, and verify `data/sample/known_price_cases.csv` cases.
4. Mark Phase 1 **complete only when tests actually run and pass**.
5. Start investigating usable Treasury **curve** data during Phase 3, even though data cleaning is scheduled for Week 6.

## Tracking rules

When closing a phase, record commit SHA, commands used, reference calculation, tolerance, assumptions and reviewer (if applicable). Do not infer progress from repository scaffolding or documentation alone.

See [eight-week roadmap](docs/07_eight_week_roadmap.md), [validation plan](docs/06_validation_plan.md), and [decision log](docs/09_decision_log.md).

