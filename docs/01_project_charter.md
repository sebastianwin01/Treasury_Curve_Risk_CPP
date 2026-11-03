# 01 — Project charter

## Working title

**Treasury Yield Curve Trading and Risk Analysis in C++**

Previous working title included **Regime-Aware**. Because advanced regime inference and ML are excluded, the simpler title accurately describes the core deliverable. A descriptive non-ML regime comparison may be added later, with the original wording restored only if the results justify it.

## Main question

> How do changes in the U.S. Treasury yield curve affect the risk and P&L of DV01-balanced 2-year/10-year relative-value positions?

## Subquestions

1. How do coupon payments, interest rates and maturity determine the present value of a fixed-rate bond?
2. How accurately do DV01 and duration/convexity approximate actual repricing under rate shocks?
3. How must notionals be set so long 2Y and short 10Y legs have similar DV01 magnitudes?
4. What happens to such a trade under parallel rate moves, steepening and flattening?
5. What is the measured historical P&L after reasonable, explicit coupon, aging, rebalancing, financing and cost assumptions?
6. Which conclusions are robust to changing these assumptions?

## Academic and technical goals

- Apply present value, basic fixed-income mathematics, calculus and numerical methods.
- Demonstrate readable, tested C++20 with appropriate use of the standard library and RAII.
- Bridge math with economic interpretation; avoid copying large quantities of finished code without deriving it.
- Make the work reproducible from documented inputs, configurations and test cases.

## Deliverables

1. Mathematical notes and specified assumptions.
2. Small validated C++ bond-pricing and risk functions.
3. Readable data-cleaning and curve inputs.
4. DV01-neutral 2Y–10Y scenario engine.
5. Historically informed research results with limitations; no claim of executable profits from fitted curves alone.
6. Final charts, technical report, README and FQE presentation.

## Team/process

Roles, specific presentation date and GitHub repository URL remain unconfirmed in the provided project context; do not invent these details. Use a decision log for agreements and owners.

## Exclusions

- Machine learning, neural networks, ML forecasting, HMMs and Markov-switching regimes.
- Unvalidated or speculative profitability claims.
- Broker integration, actual trading, derivative and credit models.
- Overengineered class hierarchies or unnecessary dependencies in the first phase.

## Success criteria

A reviewer can run documented checks, reproduce benchmark bond pricing and risk results, understand exactly how trade positions are sized, and trace each historical result back to data and assumptions.
