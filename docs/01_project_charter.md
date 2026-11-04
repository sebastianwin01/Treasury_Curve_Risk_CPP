# 01 — Project charter (MVP)

## Working title

**Treasury Yield Curve Trading and Risk Analysis in C++**

## One-sentence outcome

Build a **small, tested C++20 application** to price fixed-coupon Treasury-style bonds, measure duration/convexity/DV01, form a **DV01-balanced 2Y–10Y steepener or flattener**, and compare P&L under **instantaneous hypothetical interest-rate shocks**.

## Research question

How do parallel and nonparallel changes in the 2Y and 10Y yields affect the risk and **theoretical immediate P&L** of first-order DV01-neutral Treasury-style bond positions?

## Required demonstration

- Two hypothetical fixed-rate bonds with regular semiannual coupons.
- Price from YTM; recover YTM by bisection.
- Macaulay and modified duration, convexity and DV01.
- Balanced long/short 2Y–10Y positions.
- Parallel rise, steepening and flattening scenarios; compare first-order P&L and full repricing.
- Independently validated C++20/CMake/CTest results with financial interpretation.
- Brief written report and final FQE presentation.

## Mandatory assumptions and boundaries

Synthetic **fixed-coupon Treasury-style** examples, $100-face quotation basis, nominal annual YTMs and regular semiannual payments. Parallel shocks mean equal changes in the **two instrument YTMs**. The scenario holds instrument cash flows and remaining maturities constant while bumping yields; no passage of time, accrued interest, financing, rebalancing, coupons during the holding period, transaction costs or actual prices.

The baseline is **risk/scenario research**, not a historical trading P&L or proof of alpha. No ML, Markov-switching, forecasting, brokerage integration or live execution.

## Stretch, not a completion requirement

A simple **zero/discount curve** and descriptive **historical Treasury par-yield spread** may be added **after passing the MVP**. A dated bond portfolio simulation with short borrow, funding, accrual, coupons and rebalancing is a **separate future extension**, not the eight-week requirement.

## Constraints and acceptance

Plan for about **5–7 hours/week over eight weeks**, including reading, implementation and testing. A reviewer should clone, build and reproduce the numerical benchmark and scenario table without external market data. Quality and reproducibility take priority over feature count.

Progress belongs in [../PROJECT_STATUS.md](../PROJECT_STATUS.md); API specification: [11_technical_architecture.md](11_technical_architecture.md).
