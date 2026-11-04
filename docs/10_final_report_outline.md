# 10 — Final FQE report and presentation: minimum required

## Suggested 7-slide outline

1. **Question and scope:** interest-rate-shock sensitivity of a hypothetical 2Y–10Y DV01-balanced trade; no trading-profit claims.
2. **Bond math:** $100-face fixed-coupon bond, cash flows, YTM discounting, benchmark $98.1190129.
3. **Risk math:** duration, convexity, DV01, finite-difference validation.
4. **Strategy construction:** long 2Y/short 10Y steepener, reverse flattener, matched dollar DV01 with units.
5. **Results:** parallel, steepening and flattening immediate YTM shocks; first-order versus exact two-bond repricing.
6. **Engineering and validation:** C++20 modular code, CMake/CTest, numerical tests and reproducibility commands.
7. **Interpretation and limitations:** convexity residual, model simplifications, differences between hypothetical scenario P&L and executable historical trading, optional next work.

## Minimum exhibits

- One cash-flow/price calculation showing the independent reference.
- One leg-DV01 balancing example with correct notional units.
- One scenario table with approximate and full-repricing P&L.
- One short table of tests and pass/fail evidence.

## Optional illustration

A historical plot of Treasury **10Y minus 2Y par yields** is permitted only as *market context*, not historical returns on the hypothetical strategy. A full zero-curve or dated historical bond P&L is out of the required eight-week scope.

## Required statement

"This project measures theoretical immediate price sensitivity for synthetic Treasury-style bonds, using clearly stated YTM and rate-shock conventions. It does not demonstrate real executable performance or investment profitability."
