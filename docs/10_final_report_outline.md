# 10 — Final report / FQE presentation outline

1. **Title and team:** Treasury Yield Curve Trading and Risk Analysis in C++.
2. **Research question:** Effect of U.S. 2Y–10Y curve changes on DV01-balanced trade risk and P&L.
3. **Financial foundations:** Treasury coupons, present value, YTM, yield curves, DV01 and convexity.
4. **Methods:** C++20 architecture, pricing assumptions and risk calculations.
5. **Benchmark validation:** $98.119 pricing example, par test, YTM recovery and finite-difference risk checks.
6. **Trading construction:** Why long 2Y/short 10Y is a steepener; how matching DV01s sets notionals.
7. **Scenario results:** Parallel shifts, steepening and flattening; first-order approximation vs. full repricing.
8. **Historical data:** Precisely which series, conventions and instruments were used; any missing observations.
9. **P&L and risk:** Descriptive plots and simulated/reproducible results, with costs and limitations.
10. **Robustness and limitations:** Model and market-data assumptions; what is and is not executable.
11. **Conclusions:** What was established, what remained uncertain, and reasonable future improvements.

## Figures worth generating

- Bond price as a function of YTM.
- Yield-curve slope over the sample period.
- 2Y and 10Y DV01 leg exposure under chosen portfolio weights.
- P&L response to curve shock grid.
- If genuinely supported by data: historical simulated P&L and drawdown, with funding/cost scenarios.

## Language to use carefully

Use **hypothetical**, **scenario**, **synthetic** or **historical** accurately. Do not label an approximation as an observed return; do not imply predictability or profitable alpha without empirical evidence. No ML claims or regime model claims.
