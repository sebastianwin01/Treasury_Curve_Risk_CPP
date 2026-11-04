# 02 — Mathematics and conventions (MVP)

These equations define the **hypothetical regular-coupon, two-YTM shock model** only. They do not specify real Treasury clean/dirty prices, settlement dates or a fitted zero curve.

## 1. Bond and coupon cash flows

For face `F > 0` (USD), annual fixed coupon rate `c ≥ 0` (decimal), coupons per year `m > 0`, and maturity `T > 0` years:

- Require `N = mT` to be a **positive integer** (regular coupon periods).
- Payment at period `k` is `CF_k = F*c/m + (k == N ? F : 0)`.
- Payment time is `k/m` years, for `k = 1,...,N`.

The last payment includes principal. There are no dated settlement or holiday conventions in this exercise.

## 2. Price from nominal annual YTM

```math
P(y)=\sum_{k=1}^{N}\frac{CF_k}{(1+y/m)^k}
```

Require `1+y/m > 0` and finite numeric inputs. This is a **single-bond YTM** calculation.

**Independent test:** `F=100`, `c=0.04`, `T=2`, `m=2`, `y=0.05` gives four payments `(2,2,2,102)` and:

```text
P = 2/1.025 + 2/1.025^2 + 2/1.025^3 + 102/1.025^4
  ≈ 98.1190129 USD
```

For the same bond at `y=0.04`, `P=100`. For a conventional positive-cash-flow bond, price falls when yield rises.

## 3. Recover YTM by bisection

Given a positive target price `P_market`, solve `P(y)-P_market=0` inside a valid bracket with opposite endpoint signs. Enforce a maximum iteration count and price/yield tolerances; reject invalid/unbracketed problems instead of returning a guessed yield.

## 4. Yield risk definitions

For `P=P(y)` with yield `y` expressed as **annual decimal**:

```text
ModifiedDuration = -(1/P) * (dP/dy)          [years]
DV01             = -(dP/dy) * 0.0001        [USD per 1bp]
Convexity        = (1/P) * (d²P/dy²)        [years²]
```

Macaulay duration for regular cash flows:

```math
D_{\mathrm{Mac}}=\frac{1}{P}\sum_{k=1}^{N}\frac{(k/m)\,CF_k}{(1+y/m)^k}
```

Under the same compounding, `D_mod = D_Mac / (1+y/m)`.

Second-order price approximation for a change `Δy` in **annual decimal**:

```math
\Delta P \approx -P D_{\mathrm{mod}}\Delta y+\tfrac12 P C(\Delta y)^2.
```

Finite-difference **dollar DV01 for a 1bp move** with `h=0.0001`:

```text
DV01_fd ≈ [P(y-h) - P(y+h)] / 2
```

The denominator is `2` for **dollars per 1bp** (as opposed to dividing by `2h` for dollars per unit annual decimal yield). Compare analytic metrics with finite-difference checks under sensible tolerances.

## 5. Two-bond DV01 balancing and scenario P&L

Let `q` denote a **signed face notional** in USD (+ long, − short), `F` the face amount used to quote the bond price, and `d` its **positive** DV01 per `F`.

- Total signed exposure magnitude per leg: `q/F * d`, preserving the sign of `q`.
- For **long 2Y / short 10Y**, choose absolute face sizes so `|q2|/F2 * d2 = |q10|/F10 * d10`.
- An instantaneous yield shock of `b` basis points corresponds to `Δy = b / 10000`.

First-order hypothetical P&L:

```text
PnL_approx = -(q2/F2)*d2*b2 - (q10/F10)*d10*b10
```

Exact (under the two-YTM model) repricing:

```text
PnL_full =
    (q2/F2)   * [P2(y2+b2/10000)   - P2(y2)]
  + (q10/F10) * [P10(y10+b10/10000) - P10(y10)]
```

The cash flows and remaining maturity are held constant across the immediate shock. A DV01-balanced trade has **approximately zero first-order P&L** from equal yield moves, but full repricing can leave a **convexity residual**.

## 6. Hard boundaries

This is **not** equivalent to a fully specified coupon-date zero-discount curve or a historical portfolio backtest. Published **Treasury par yields** are not zero-coupon rates, individual-bond YTMs or executable prices. The MVP deliberately omits settlement calendars, accrued interest, financing, coupon cash during a holding period and rebalancing.

Implementation strategy: use `double`, `std::vector`, explicit input checks, no owning raw pointers, and tolerance-based floating comparisons. See [03_trading_strategy.md](03_trading_strategy.md) and [06_validation_plan.md](06_validation_plan.md).
