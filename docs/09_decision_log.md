# 09 — Decision log and open conventions

This file distinguishes **confirmed project scope** from **proposed experimental defaults** and **open decisions**. Proposals are not final financial conventions and should not be represented as implemented behavior.

## Confirmed scope decisions

| Topic | Decision | Status |
|---|---|---|
| Project | Treasury Yield Curve Trading and Risk Analysis in C++ | Confirmed |
| Research focus | Risk and P&L of DV01-balanced 2Y–10Y U.S. Treasury steepener/flattener positions | Confirmed |
| Financial instruments | Nominal fixed-rate Treasury-style bonds in the baseline | Confirmed |
| Programming | C++20 for financial analytics; optional Python for non-ML data handling and plots | Confirmed |
| Modeling boundary | **No machine learning or Markov-switching models** | Confirmed |
| Trading boundary | Research/paper simulation, not live execution or a profit claim | Confirmed |
| Core deliverables | Bond pricing, YTM, duration, convexity, DV01, 2Y–10Y size/risk, deterministic shocks and validation | Confirmed |
| Historical research | Conditional **synthetic** P&L if data and assumptions support defensible valuation | Confirmed as target, not yet implemented |
| Optional regime comparison | Transparent retrospective rule-based descriptions only after baseline works | Optional |
| Work plan | Eight phases with Phases 4 and 5 sharing Week 5; total about 5–7 h/week | Planning assumption, not deadline guarantee |
| First benchmark | $100 face, 4% coupon, 2 years, semiannual; YTM 5% → price ~ $98.1190129 | Independently specified, not yet a passed software test |

## Phase alignment (not proof of completion)

1. Bond pricing fundamentals — Week 1.
2. Yield solving and risk sensitivities — Weeks 2–3.
3. Treasury yield curves — Week 4.
4. 2Y–10Y portfolio construction — first half Week 5.
5. Scenario and stress analysis — second half Week 5.
6. Historical Treasury data — Week 6; feasibility assessment starts in Phase 3.
7. Historical P&L and backtesting — Week 7 **only if data/valuation permit**.
8. Final integration and research report — Week 8.

Actual implementation progress belongs in [../PROJECT_STATUS.md](../PROJECT_STATUS.md). No source code should be assumed complete because a phase has been planned.

## Confirmed first-exercise pricing convention

For the **teaching benchmark only**:

- Face `F = $100`; annual coupon rate `c = 0.04`; maturity `T = 2` years.
- Semiannual coupons, `m = 2`; four regular periods and cash flows `(2, 2, 2, 102)`.
- Nominal annual YTM `y = 0.05` with **semiannual compounding**; annual rates stored as decimal numbers.
- Price formula: `P(y) = SUM CF_k / (1+y/m)^k`.
- Expected price `P(0.05) ≈ 98.1190129`; par `P(0.04) = 100`.
- This simplified calculation does **not** establish real Treasury settlement, accrual/day-count, calendar or quote conventions.

## Proposed first synthetic-research conventions (not yet locked)

| Topic | Initial proposal | Why it needs validation/approval |
|---|---|---|
| Synthetic instruments | One fixed-rate 2Y and one fixed-rate 10Y Treasury-style coupon bond created at each chosen entry/roll | Avoid treating rolling constant-maturity series as the same aging bond |
| Quotation unit | $100 face value with consistent scaling to actual modeled notionals | Avoid DV01 and P&L unit confusion |
| Coupon schedule | Semiannual, fixed coupon set **at inception** (e.g., from a consistent initial par calculation) | The coupon must not change as yields change |
| Valuation | Sum remaining cash flows times coupon-date discount factors `D_t(T_k)` | Par yields cannot simply substitute for zero rates |
| Price/interest treatment | Prefer **dirty (full) price** mark and record actual coupon cash on payment dates | Prevent double counting accrued interest |
| Holding policy | Fixed predetermined holding interval, explicit exit; rolling/rebalancing as future variants | A 10Y bond ages; synthetic fixed-tenor replacement is a transaction |
| DV01 hedge | Match magnitude under one explicit parallel-bump and interpolation convention | Parallel neutrality is first-order only |
| Financing | Parameterized annual cash/borrow rate, separately parameterized short-security borrow cost | Must avoid undocumented free leverage or free short proceeds |
| Execution | Symmetric stated entry/exit/rebalance spread or cost on each leg | Costs are assumed, not observed trade executions |
| Capital/returns | State capital/haircut explicitly; otherwise report dollar P&L without claiming ROE | Avoid meaningless percent returns |
| Historical labels | Every fitted-curve backtest labeled **synthetic** or **simulated** | Modeled values are not executable market quotes |

Detailed algorithmic and accounting proposals: [05_backtest_methodology.md](05_backtest_methodology.md).

## Open decisions before historical P&L

- [ ] Verify that usable historical zero/discount curve inputs exist for chosen dates and terms; identify source, publication lag and license.
- [ ] Select curve compounding and interpolation (discount factor, log-discount, or zero-rate interpolation) with boundary behavior.
- [ ] Choose actual synthetic inception coupon rule, start dates, settlement convention, business-day rule and year fractions.
- [ ] Choose coupon schedule, clean/dirty/accrued interest accounting and how coupon days appear in the daily ledger.
- [ ] Choose whether instruments are held until a predetermined date, rebalanced periodically, or rolled into new synthetic issues.
- [ ] Define precisely which curve bump constitutes 'parallel' and how leg DV01 is measured.
- [ ] Decide portfolio cash/collateral bookkeeping, financing reference/compounding, short borrow/rebate rate and haircut.
- [ ] Decide per-leg transaction-cost assumptions, leverage or notional constraints and relevant return denominator.
- [ ] Choose historical sample dates and evaluation protocol **before** examining simulated returns.
- [ ] Confirm that any historical result is supportable; otherwise commit to **scenario-only plus descriptive history**.
- [ ] Assign collaborator responsibility and agree final presentation date.

## Changes adopted in this documentation revision

**Date:** 2026-11-04  
**Decision:** Refine the existing eight-phase roadmap without adding ML or changing the core 2Y–10Y objective.  
**Options considered:** Keep earlier combined strategy/shock milestones; split portfolio construction from scenario stress; require historical returns unconditionally.  
**Rationale:** Separate mathematical position sizing from validation through shocks; make synthetic-history feasibility a go/no-go step instead of an unsupported promise.  
**Impact:** README, status, roadmap and backtest methodology now distinguish implemented work, deterministic scenarios, synthetic-instrument modeling and financing/coupon assumptions.  
**Status:** Documentation decision adopted; all engineering milestones remain to be implemented and independently validated.

## New decision record template

**Date:** YYYY-MM-DD  
**Decision:** ...  
**Status:** Confirmed / Proposed / Open  
**Alternatives:** ...  
**Rationale:** ...  
**Data and convention impact:** ...  
**Implementation/tests impacted:** ...  
**Owner/reviewer:** ...  
**Validation evidence / commit SHA:** ...

Do not silently promote proposed defaults into confirmed decisions or treat modeled scenarios as realized trading performance.

