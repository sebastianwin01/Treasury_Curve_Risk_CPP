# 04 — Data plan: minimum inputs first

## Required for the MVP — no download dependency

The **completed core must run offline** using small, synthetic input cases:

- `data/sample/known_price_cases.csv` contains independent $100-face pricing benchmarks.
- `data/sample/illustrative_trade_scenarios.csv` stores the three midpoint-slide **assumed-DV01** linear P&L examples.
- Additional 2Y/10Y coupon/yield inputs may be hard-coded in focused unit tests or saved to a small reproducible CSV.

These benchmarks are **not** current market quotes and should be clearly labeled.

## Optional: descriptive 2Y–10Y Treasury history

A simple plot of the 10Y minus 2Y **par yield spread** is useful context, but **not necessary to finish** the C++ risk engine.

Possible public source: [U.S. Treasury interest-rate statistics](https://home.treasury.gov/policy-issues/financing-the-government/interest-rate-statistics). Verify the units, definitions, covered dates and source revisions when data are actually acquired.

Proposed normalized data:

```text
date,y2_par_decimal,y10_par_decimal,spread_bp
```

Compute `spread_bp = 10000 * (y10_par_decimal - y2_par_decimal)`. Handle missing observations by reporting or skipping gaps; **do not fabricate weekend/holiday trading observations**.

## Financial distinction — essential

**Par yields ≠ bond YTM ≠ zero-coupon rates ≠ bond-specific executable quotes.** Descriptive par spreads are acceptable as historical context, but are **not sufficient** to mark coupon bonds at dated market prices or to claim actual portfolio P&L.

A future **zero-curve** extension would need maturity-specific curve inputs and well-specified compounding/interpolation. The [Fed fitted Treasury yield curve](https://www.federalreserve.gov/data/nominal-yield-curve.htm) may be considered later, with its specific series and estimation assumptions documented.

## Go/no-go

- **Always deliver MVP:** numerical pricing, risk and stress tests with entirely synthetic inputs.
- **If time permits:** optional descriptive historical spread chart with provenance and no strategy-return claims.
- **Never required:** a fully financed, coupon-aware historical bond simulation.

More detailed future requirements are explained in [05_backtest_methodology.md](05_backtest_methodology.md).
