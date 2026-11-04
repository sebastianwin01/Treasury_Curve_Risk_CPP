# 05 — Synthetic backtesting and research methodology

## Research purpose and scope

**Question:** How do Treasury yield-curve changes affect the risk and simulated P&L of DV01-balanced 2Y–10Y steepener and flattener positions?

A change in the quoted 10Y–2Y spread **is not itself an executable trade return**. Position P&L requires bond-specific or synthetic-instrument cash flows, sign and size of each leg, a valuation convention, passage of time, coupons, funding, rebalancing and transaction costs.

The project has **no machine learning, no predictive trading signal, and no live trading**. The central output is validated scenario/risk analysis; historical P&L is a conditional extension.

## Three distinct evidence levels

| Level | Name | What is permitted | What must not be claimed |
|---|---|---|---|
| 1 — Required | **Deterministic scenarios** | Hypothetical 2Y/10Y bonds, DV01 matching, immediate rate shocks, first-order approximations versus full repricing | Historical profits, executable prices or forecasting performance |
| 2 — Conditional target | **Synthetic historical valuation** | Fixed, prespecified synthetic bond contracts aged through observed dated yield/discount curves; assumed costs | Realistic executable fills or verified actual-market returns |
| 3 — Future only | **Actual-security P&L** | Identified Treasury securities with dated quotes, settlement, cash flows and borrow/funding evidence | Anything equivalent to Level 3 without actual instrument-level data |

All figures and tables must state which level they represent. A synthetic P&L series is **not a realized trading record**.

## Data and rate conventions

- Daily U.S. Treasury **constant-maturity par yields** can describe the 2Y–10Y spread, but must **not** be silently used as cash-flow-date zero rates or as actual bond transaction prices.
- A dated **zero-rate / discount-factor curve** may support modeled cash-flow PVs if its estimation method, maturities, rate units, compounding and interpolation are documented and tested.
- Start with C++ benchmark bonds using nominal annual YTM with semiannual compounding. For curve valuation use `P(t) = SUM_k CF_k * D_t(T_k)`, where `D_t(T_k)` discounts the future payment date `T_k` from observation date `t`.
- Normalize quoted percent/basis points to **decimal annual rates** internally and label all monetary outputs by face-value/notional units.
- Record source URL, observation timestamp, publication/availability information where relevant, acquisition date, treatment of missing dates and tenors, and curve interpolation.
- If curve-input suitability cannot be demonstrated, limit historical work to **descriptive spread analysis**; do not create a pretend portfolio return series.

## Synthetic instrument definition — proposed, not yet approved

These are **proposed conventions for a reproducible first experiment**, not claims about actual Treasury issues and not yet finalized project decisions:

1. **Initial instruments:** two hypothetical fixed-rate, nominal Treasury-style coupon bonds, one with 2 years and one with 10 years remaining at entry.
2. **Face and coupon:** choose a common valuation unit of **$100 face**. Each instrument's annual coupon is set **once at inception** using an explicitly documented rule (e.g., approximate par coupon derived from a valid inception discount curve); after inception, its coupon **stays fixed** during the holding period.
3. **Schedule:** semiannual coupon dates and principal repayment at maturity. First use regular periods; exact settlement, holidays, accrued interest and day-count implementation must be specified before claiming actual Treasury realism.
4. **Aging:** preserve the original contractual maturity and payment dates as simulation time advances. A bond purchased with 10 years remaining does **not** remain a perpetual 10-year bond unless the simulation explicitly sells it and buys a new one.
5. **Roll policy:** initially use a **fixed holding interval**, with explicit sale/mark at its end. Later choose whether to roll or rebalance monthly, at specified dates, or at a stated DV01 threshold; record the actual choice before running results.
6. **Pricing:** use the available dated curve for all remaining payment dates, under consistent interpolation/compounding. Document the treatment of coupons between observation dates.
7. **Entry/exit pricing:** modeled bond PVs are *theoretical mid-style valuations*; no actual ability to transact at these levels is implied.

**Still to decide:** exact inception coupon rule, trade-date convention, payment schedule, clean/dirty treatment, historical period, holding/rolling frequency and data source. Record choices in [09_decision_log.md](09_decision_log.md) before implementing Level 2.

## Portfolio construction and DV01 neutrality

Define each leg's DV01 per common face unit using the same rate bump and curve convention. Let `q_2` and `q_10` denote nonnegative numbers of face units and `d_2`, `d_10` the corresponding positive per-unit DV01 magnitudes.

```text
q_2 * d_2 ≈ q_10 * d_10
```

- **Steepener:** long 2Y, short 10Y. First-order P&L for a small move (yields in bp): `d * (Δy_10 - Δy_2)` with matched leg DV01 magnitude `d`.
- **Flattener:** short 2Y, long 10Y; reverse the above sign.
- This is **first-order** neutrality for the chosen parallel bump. It does not remove key-rate, convexity, funding, liquidity or basis risk.
- Specify position signs and units before any numerical example. Record whether face notionals are fixed during the holding period and when hedges are restored.

## Coupon, accrued interest and cash-ledger treatment

A reliable P&L calculation must be based on **dated cash flows** and a documented valuation convention.

**Preferred future historical convention:** use **dirty (full) bond values**, with coupons credited to the long leg and debited by the short leg on their payment dates, so accrued interest is incorporated through valuation rather than separately counted twice. If the implementation uses clean prices, explicitly add accrued interest and reconcile to dirty valuation.

For a fixed-size leg over one interval (before financing and fees):

```text
GrossPnL_long  = q * (DirtyPrice_end - DirtyPrice_start) + q * Coupons_paid
GrossPnL_short = -q * (DirtyPrice_end - DirtyPrice_start) - q * Coupons_paid
```

Here `q` scales the quotation unit consistently; include principal redemption flows when a security matures. The short position owes coupon-equivalent payments. **Do not assume zero P&L simply because market yields are unchanged**: accrued interest, carry and changes in remaining maturity matter.

For portfolio accounting:
- Track **units owned/owed**, cash, fees, financing balances, coupons, principal at maturity and all rebalancing/roll transactions.
- Mark both instruments with the **same observed date** and document the timing of cash transactions versus valuations.
- Record gross mark-to-market plus income and **net P&L after explicitly modeled expenses**.
- Require cash-ledger reconciliation: portfolio value change plus realized cash flows should agree with the P&L decomposition.

## Financing, shorting and transaction costs — assumptions required

The initial deterministic scenario may omit financing and trading costs **only when prominently labeled as an instantaneous, gross shock calculation**.

A synthetic multi-day backtest must at least define:
- **Long-leg financing:** whether purchases are fully cash-funded (and the opportunity cost treatment) or funded at a stated benchmark borrowing rate. If a repo-style rate is assumed, specify borrowing fraction/haircut and rate source.
- **Short-leg mechanics:** proceeds/collateral treatment, interest or rebate earned on proceeds if any, and security borrow/specialness costs. Do not assume that short-sale proceeds are freely available for leverage.
- **Transaction costs:** bid/ask spread or per-face execution haircut on both legs at entry, exit, and any rebalance; decide whether slippage is included separately.
- **Capital/leverage:** initial equity, margin or haircut assumptions, maximum gross notional and a clear return denominator. P&L in dollars can be reported even when defensible return-on-equity assumptions are unavailable.
- **Unobserved financing data:** document placeholder assumptions, evaluate sensitivities across plausible alternative values, and do not label any resulting net P&L as achieved market returns.

**Proposed starter simplification (subject to decision log):** one flat annualized financing-rate input applied with actual elapsed calendar time, separately specified short-borrow charge, and fixed per-trade basis-point execution cost. These are **synthetic sensitivity inputs**, not observed market rates or fills. A self-financing long/short interpretation must still reconcile cash and collateral.

## Deterministic stress tests (Phase 5)

Before loading historical observations:
1. Validate stand-alone bond prices and DV01 with manual examples.
2. Size both legs to matched initial DV01 magnitude.
3. Apply **parallel** moves, **2Y–10Y steepeners**, **flatteners**, and larger asymmetric shocks.
4. Calculate **first-order DV01**, **duration-plus-convexity**, and **full repricing** P&L under the same rate-bump convention.
5. Report absolute and relative approximation errors; identify remaining convexity and curve exposures.
6. Label numbers as **illustrative hypothetical P&L**, not backtested profit.

## Historical daily simulation loop (conditional Phase 7)

1. Read only information that was available by the decision timestamp at day `t`; validate tenors and rate conventions.
2. Define or load the **fixed existing instruments** with issue/entry date, coupon, remaining maturity and cash-flow schedule.
3. Reprice their remaining dated cash flows from the curve at `t`, measure current DV01, and determine whether a **prespecified** rebalancing rule triggers.
4. Execute simulated entry, exit or rebalancing transactions at defined synthetic marks **plus modeled costs**; post collateral and financing ledger entries.
5. Move to `t+1`, age the same instruments, credit/debit any coupon or principal cash flows and accrue the relevant financing, rebate and borrow costs over the actual interval.
6. Reprice all still-open legs, update portfolio value, ledger and exposure; reject or flag missing rate data rather than inventing trading observations.
7. Save a row with valuation date, leg identifiers, notionals, DV01s, dirty values, coupon cash, financing/borrow costs, transaction costs, cash, gross/net P&L and reconciled exposure.
8. Run the same process under alternate stated costs, roll rules and interpolation choices.

## Bias prevention, testing and outputs

- Use chronological inputs and **past-only** decision information. Lock chosen assumptions before inspecting outcomes; do not select historical periods/rules solely for attractive returns.
- Validate at least one coupon period and one rebalance/roll manually; compare cumulative P&L to the cash ledger and independent full repricing.
- Include unit tests for date advancement, maturity, coupons, DV01 sign, costs, missing yields, and no-rebalance versus rebalance paths.
- Report coverage dates, data gaps, inferred versus observed quantities, costs, gross versus net dollar P&L, drawdown and sensitivities **only when those measures are meaningfully defined**.
- State that **a synthetic strategy backtest cannot prove trading profitability**, especially without bond-specific executable prices and actual borrowing conditions.

**Go/no-go gate:** If assumptions or data cannot support dated synthetic coupon-bond prices, deliver tested deterministic stress results plus descriptive Treasury yield-history analysis—never fabricate an executable return series.

Related: [03_trading_strategy.md](03_trading_strategy.md), [04_data_plan.md](04_data_plan.md), [06_validation_plan.md](06_validation_plan.md), [09_decision_log.md](09_decision_log.md).

