# 03 — Trading strategy and exposure specification

## 1. Research trade

Study **2Y–10Y U.S. Treasury curve steepeners and flatteners**. These are relative-value bets on the change in the **10Y yield minus 2Y yield**:

`Slope_t = y10_t - y2_t` (in basis points or decimal rate, with units fixed).

- **Steepening:** `ΔSlope > 0`.
- **Flattening:** `ΔSlope < 0`.

For a **DV01-balanced steepener**, a simple directional construction is **long 2Y bonds and short 10Y bonds**, chosen so their first-order yield exposures offset for a parallel shift. This trade tends to gain (first order) when 10Y yields rise *relative to* 2Y yields.

For a **DV01-balanced flattener**, reverse the legs: **short 2Y, long 10Y**.

## 2. Hedge sizing

Let q2 and q10 be *positive* face-value position sizes, and d2 and d10 the DV01 dollar magnitudes for a common face-value unit. Choose q2 and q10 such that:

`q2 * d2 ≈ q10 * d10`.

Specify whether q is in millions of face amount or absolute dollars and express d consistently.

This neutralizes **only first-order parallel exposure under the exact bump convention used for each DV01**. It does not eliminate convexity, curve, basis, funding or instrument-specific risks.

## 3. Illustrative check from the FQE midpoint slides (NOT market data)

- Assumed 2Y DV01 per $1m face: **$190 per bp**.
- Assumed 10Y DV01 per $1m face: **$760 per bp**.
- Steepener: **long $4m face 2Y**, **short $1m face 10Y**.
- Exposure magnitude on each side: **$760/bp**.
- First-order P&L when yield changes are measured in **basis points**:

`Approx_PnL = $760 * (Δy10_bp - Δy2_bp)`.

| Scenario | Δy2 | Δy10 | Approx P&L |
|---|---:|---:|---:|
| Parallel rise | +10 bp | +10 bp | $0 |
| Steepening | +5 bp | +15 bp | +$7,600 |
| Flattening | +15 bp | +5 bp | -$7,600 |

These numbers intentionally ignore convexity, passage of time, coupons, carrying costs, transaction costs and the practical ability to borrow/short securities. **Do not represent them as market quotes or realized performance.**

## 4. Trading questions to decide and document

1. Which actual 2Y and 10Y securities (or hypothetical proxies) define the legs on each trade date?
2. Does the model continuously hold bonds as they age, roll into new issues monthly, or rebalance at other times?
3. Do we rebalance notionals each day, each month, or when DV01 deviation reaches a threshold?
4. How are purchases, sales, shorts, coupons, clean/dirty prices and funding costs accounted for?
5. Is the strategy always a long 2Y / short 10Y scenario study, or are entry/exit rules prespecified?
6. Is the research purpose *risk attribution* or a *predictive strategy*? The baseline is risk attribution; it must not be presented as proven predictive alpha.

## 5. Scope-appropriate optional regime analysis — no ML

After core research works, compare **retrospective descriptive slices** using definitions written down before examining performance:

- Normal (`10Y - 2Y >= 0`) vs. inverted (`< 0`) curve.
- Rates rising vs. falling using a documented trailing window.
- High vs. low realized yield-change volatility using a documented, past-only threshold.

This is **rule-based stratification**, not a Markov-switching or machine-learning model. If computing strategy signals, use information available at the decision time only. Do not imply that descriptive group labels can forecast markets.
