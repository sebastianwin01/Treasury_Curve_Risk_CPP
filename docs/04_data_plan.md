# 04 — Historical data plan

## Sources to investigate

1. **Federal Reserve — U.S. Treasury Nominal Yield Curve**: https://www.federalreserve.gov/data/nominal-yield-curve.htm
   - Provides fitted nominal curve information. Check the specific available series, estimation method, conventions and date coverage before choosing zero/discount inputs.
2. **U.S. Treasury — Interest Rate Statistics**: https://home.treasury.gov/policy-issues/financing-the-government/interest-rate-statistics
   - Daily constant-maturity par Treasury yields can support descriptive 2Y/10Y spread studies; they are **not** a curve of coupon-date zero rates and are **not** executed security prices.
3. For actual-security valuation (future scope), documented bond-specific coupons, maturity dates, market prices, settlement and accrual rules are needed.

## Data distinctions

- **Par yield:** yield of a hypothetical par-coupon bond with a chosen maturity. Good for simple 2Y/10Y curve level/slope monitoring.
- **Zero yield:** directly related, after compounding specification, to discount factors for *individual dates*.
- **Bond YTM:** one rate solving the market price of a particular coupon bond.
- **Tradable price:** price of a specific Treasury instrument at a specific observation time; not supplied directly by a fitted yield alone.

**Never treat any of these as interchangeable without a documented transformation.**

## Proposed raw schema

`date,source,series_type,maturity_years,yield_percent,compounding,observation_status`

- Date in ISO `YYYY-MM-DD`.
- `series_type`: `par`, `zero`, or another explicitly described convention.
- Store original quoted units and add normalized decimal-rate fields during cleaning.
- `observation_status` flags source-identified missing or unavailable observations.

## Proposed clean long-form schema

`date,source,maturity_years,rate_decimal,rate_type,compounding,quality_flag`

For a 2Y/10Y descriptive study, pivot to a consistent daily pair only if both tenors are available on that day. Preserve missing observation flags; **do not invent weekend or holiday observations**.

## Data validation

- Log acquisition date, dataset/source URL, filters, parsing code, and checksum if possible.
- Confirm dates are monotonic after sorting; remove exact duplicates deterministically.
- Validate yield units: source percentage `4.2` should be normalized to decimal `0.042`, not `4.2`.
- Track missing periods/tenors, definition changes, source revisions and suspicious outliers.
- Exclude look-ahead data from every rule used in a historical backtest.
- Keep `data/raw` and `data/processed` files untracked by default unless licensing and size permit redistribution.

## Minimal milestones

**First:** inspect a small sample of daily 2Y and 10Y par yields to compute slope, trend and changes.

**Then:** choose and document a curve representation appropriate for bond repricing, potentially using a fitted zero/discount curve plus interpolation. Validate against independent calculations and document approximation limitations.

**Finally:** determine whether you can support actual-instrument historical P&L. Otherwise call the results *hypothetical curve-shock P&L* or *synthetic valuation*, not executable returns.
