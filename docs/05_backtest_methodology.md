# 05 — Methodology: immediate stress scenarios, not a historical backtest

## Required MVP methodology

Our core experiment is an **instantaneous repricing study** of two **hypothetical coupon bonds**, not a time-series trade execution simulation.

1. Define a fixed-coupon **2-year** bond and a fixed-coupon **10-year** bond.
2. Choose and document each bond's starting YTM; calculate bond prices and **absolute DV01 per $100 face**.
3. Choose one leg's face notional and size the opposite leg so dollar DV01 magnitudes match at the starting yields.
4. For a given scenario, set `y2_new = y2 + Δy2_bp / 10000` and `y10_new = y10 + Δy10_bp / 10000`.
5. **First-order P&L:** sum the signed leg contributions `-side * DV01_dollars_per_bp * Δyield_bp`, where `side = +1` for long, `-1` for short.
6. **Exact repricing P&L:** for each leg, `side * faceScale * (priceFromYTM(newYield) - priceFromYTM(originalYield))`. Face scale is `faceNotionalUsd / bond.faceValueUsd`.
7. Compare the approximation with full repricing and explain why residual convexity means a parallel move may not have exactly zero **full-repricing** P&L.

This is a **two-instrument YTM shock**, not a full fitted zero-curve shock. The experiment assumes **zero time elapsed**, so the same future coupons are used before and after the shock.

## Required scenarios

| Scenario | Δ2Y | Δ10Y | First-order steepener expectation |
|---|---:|---:|---|
| Parallel rise | +10bp | +10bp | Approximately 0 |
| Steepening | +5bp | +15bp | Positive |
| Flattening | +15bp | +5bp | Negative |

For the midpoint illustration, assumed DV01s of **$190/bp per $1m 2Y** and **$760/bp per $1m 10Y**, with **long $4m 2Y / short $1m 10Y**, produce **$0 / +$7,600 / -$7,600** in *first-order* P&L. That illustrative dataset is **not an exact repricing benchmark** for a particular coupon schedule.

## Explicit exclusions

No actual security identifiers, traded market prices, clean/dirty quote conventions, dated coupons during a holding period, bond aging, bid/ask spread, transaction costs, repo financing, security borrowing, collateral, hedge rebalancing, NAV accounting or historical returns.

**Do not describe scenario P&L as daily profit, strategy return or market-tested alpha.**

## Optional context versus future work

**Optional now:** display historical 2Y–10Y Treasury par-yield spread as **context only**, with source and units. Do not treat those series as coupon-bond prices.

**Separate later project:** synthetic or actual-security backtesting would require fixed dated coupon schedules, zero/discount-curve valuation, accrued interest, purchases/sales, short coupon payments, borrow/repo rates, cash ledger, rebalancing rules and cost assumptions. Until that is actually implemented and validated, **no historical portfolio P&L claim is supportable**.

See [04_data_plan.md](04_data_plan.md), [06_validation_plan.md](06_validation_plan.md) and [09_decision_log.md](09_decision_log.md).
