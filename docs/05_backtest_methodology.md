# 05 — Backtesting and research methodology

## Research principle

A change in a 2Y–10Y yield spread **does not by itself equal a trade's P&L**. The project must convert yield moves into changes in specific priced instruments, then account for position signs, DV01, convexity, coupons, aging, rebalance trades and costs.

## Baseline implementation levels

**Level 1 — Deterministic scenarios (required):** Fixed hypothetical positions, first-order DV01 P&L and full bond repricing under parallel, steepener and flattener shocks.

**Level 2 — Synthetic historical curve analysis (target):** Observe historical 2Y/10Y curves and infer hypothetical position values using clearly defined synthetic instruments and valuation assumptions. Label every chart as simulated or synthetic.

**Level 3 — Actual-security P&L (future extension):** Use security identifiers and instrument-specific historical quotations and cash flows if accessible and appropriately licensed. Not required for the baseline.

## Proposed daily research loop

1. At date t, load only market information available at the decision time.
2. Identify the 2Y/10Y instruments or synthetic proxies, time-to-maturity and coupons.
3. Price both legs and compute DV01 in dollars/bp under one consistent bump rule.
4. Choose long and short face amounts with matching DV01 magnitudes.
5. Apply any prespecified holding/rebalancing rule and a transaction-cost assumption.
6. Move to the next observation, age instruments by elapsed calendar time, account for all coupons paid and financing/short-borrow costs, then reprice both legs.
7. Record portfolio market value, cash balance, gross/net P&L, realized/unrealized contributions, exposure and margin/capital assumptions.
8. Check deviations from DV01 neutrality and evaluate stress shocks.

## P&L accounting

For a **long** position with face amount q relative to the quotation unit:

`PnL_long = q * (P_end - P_start) + coupon_cash_received - financing_costs - trade_costs`

For a **short** position, the price and coupon signs reverse and borrow/financing costs still matter. Be explicit whether P uses dirty or clean quotations so accrued interest is not double-counted. Include sale/purchase cash flows when notional changes or securities roll.

Attribution candidates: mark-to-market move, carry/coupon, roll-down, financing and execution costs. A simplified version may report fewer components but must disclose omissions.

## Data leakage and research bias protections

- Use strictly chronological information. Never let a future rate determine current position sizing, a trigger or a volatility threshold.
- Separate design/calibration period from the final evaluation period if tuning rules.
- Avoid hindsight selection of instruments, entry dates, window lengths or scenarios to maximize backtest P&L.
- Report time coverage, sample size, gaps and exact strategy decision rules.
- Treat missing values cautiously; never quietly interpolate across holidays to fabricate tradable observations.

## Required outputs

- Daily portfolio DV01 and both leg notionals.
- Scenario P&L from DV01 approximation vs. full repricing.
- Gross vs. assumed net cumulative P&L.
- Summary of return convention, risk, maximum drawdown, and downside/stress behavior (only where meaningful and correctly funded).
- Sensitivity to costs, rebalancing frequency, instrument selection and curve interpolation.
- Assumptions, limitations and a reproducible command/recipe.

**No claim that a strategy works unless an appropriate, validated out-of-sample empirical study supports it.**
