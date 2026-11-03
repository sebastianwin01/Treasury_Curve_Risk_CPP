# 06 — Testing and validation

Each test has a reason, target result and tolerance. Introduce automated C++ tests after implementing the smallest pricing function; do not mark tests passed until they actually run.

## Milestone 1 — Single-YTM bond pricer

| Test | Input | Expected behavior |
|---|---|---|
| Discount coupon bond | Face=100, coupon=4%, years=2, m=2, YTM=5% | P≈98.1190129 |
| At par | Same, YTM=4% | P=100 within tolerance |
| Zero coupon | Face=100, coupon=0, years=2, m=2, YTM=5% | P=100/(1.025)^4 |
| Yield monotonicity | Same bond, y=4%,5%,6% | P4% > P5% > P6% |
| Last cash flow | Face=100, coupon=4%, m=2 | Final payment =102 |
| Bad input | Negative face, zero m, invalid compounding base | Explicit error/contract |

Initially compare money amounts with an absolute tolerance of `1e-8` dollars for simple $100 examples, if numeric implementation permits. Tolerances must be reconsidered for larger notionals.

## Milestone 2 — Yield root finder

- Recover 5% from P≈98.1190129 (within a documented yield tolerance).
- Recover 4% from P=100.
- Fail explicitly for invalid bracket, unreachable target or missing cash flows.
- Verify that the bisection price residual is appropriately small at termination.

## Milestone 2 — Duration, convexity and DV01

- DV01 positive for a typical long positive-cash-flow fixed-rate bond.
- Duration positive and less than/equal to maturity for a conventional fixed coupon bond at positive yield (special edge cases need documented assumptions).
- Compare analytic first and second derivative results with carefully chosen central differences.
- Compare duration-only, duration+convexity, and actual repriced P&L for ±1bp and ±100bp moves.

## Milestone 3 — Curve

- Known discount-factor case: choose arbitrary independent values D(t_k), and verify `Σ CF_k D(t_k)` by hand.
- Confirm zero-rate/discount conversion is consistent with specified compounding.
- Document how interpolation affects prices and verify monotonicity/boundaries only when assumptions justify it.

## Milestone 4 — Trade sizing

With illustrative per-$1m DV01s `2Y=$190/bp`, `10Y=$760/bp`:

- Long $4m 2Y and short $1m 10Y: first-order **net parallel DV01 ≈0**.
- Δ2=+5bp, Δ10=+15bp: **approx +$7,600**.
- Δ2=+15bp, Δ10=+5bp: **approx −$7,600**.
- Both yields +10bp: **approx $0** (first order only).
- Reprice full position and observe possible convexity residuals.

## Milestone 5–7 — Historical consistency

- Reject duplicate valuation dates and report missing rate tenors.
- Reconcile position inventory, coupon schedules and cash ledger.
- Check that flat-rate/flat-price baselines produce expected carry and financing behavior, not an assumed zero P&L.
- Recalculate a sample day manually including costs and coupon accrual.
- Validate no future data enters current decisions or rule parameters.
- Reproduce selected results on a clean build/machine from documented datasets.

## Test organization

Create C++ test files under `tests/` in later milestones, connect them with CTest, and include a machine-readable benchmark CSV under `data/sample/`. The current root CMake configuration deliberately only builds the scaffold CLI.
