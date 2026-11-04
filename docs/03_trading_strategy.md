# 03 — DV01-balanced 2Y–10Y trade and scenarios

## Question

How does a **hypothetical** long/short Treasury-style bond pair respond immediately to separate changes in the 2Y and 10Y **bond YTMs**?

Define the slope as `y10 - y2`. A positive change is **steepening**; a negative change is **flattening**.

## Direction and hedge

- **Steepener:** **long 2Y**, **short 10Y**. For a small steepening shock it generally gains first order when the positions' initial dollar DV01 magnitudes match.
- **Flattener:** **short 2Y**, **long 10Y**; reverse the sign.
- **DV01 balancing:** equalize the *absolute dollar DV01* of each leg, measured under the same 1bp YTM bump convention.

For positive leg face sizes `Q2`, `Q10` and DV01 magnitudes quoted per face unit `F2`, `F10`:

```text
(Q2/F2) * DV01_2 = (Q10/F10) * DV01_10
```

The signed net exposure to a common **+1bp YTM shock** is near zero to first order. This does **not** remove convexity risk, and it is **not** a statement about all market curve factors.

## Prescribed immediate scenarios

| Scenario | Δ2Y YTM | Δ10Y YTM | First-order steepener result |
|---|---:|---:|---|
| Parallel rise | +10bp | +10bp | Approximately $0 for a balanced trade |
| Steepening | +5bp | +15bp | Positive |
| Flattening | +15bp | +5bp | Negative |

The **exact** result is calculated by revaluing both fixed bonds at their new YTMs, with no time elapsed; it can differ from the first-order result.

## Independent illustrative midpoint example

The slide example makes the following **assumptions**, not market observations:

- 2Y bond DV01 **$190/bp per $1m face**.
- 10Y bond DV01 **$760/bp per $1m face**.
- Long **$4m** 2Y; short **$1m** 10Y, giving **$760/bp** magnitude on each leg.
- First-order P&L = `$760 × (Δy10_bp - Δy2_bp)`.

The three results are **$0**, **+$7,600**, **−$7,600**. See [illustrative_trade_scenarios.csv](../data/sample/illustrative_trade_scenarios.csv).

These illustrative DV01s are **not targets for the actual coupon-bond risk function**. The example checks **hedge sizing and sign arithmetic**, not exact bond repricing.

## Scope limits

This project does not implement trading signals, real-security quotations, financing, borrowing, aging, cost accounting or historical P&L. All output is **hypothetical immediate scenario analysis**. Further formulas: [02_math_and_conventions.md](02_math_and_conventions.md).
