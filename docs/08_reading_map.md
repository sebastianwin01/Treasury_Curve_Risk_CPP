# 08 — Reading map: Tuckman & Serrat, 4th edition (2022)

Printed book page numbers, not PDF viewer pages.

## Read first, in coding order

1. **Chapter 1, pp. 49–64 — Prices, Discount Factors, and Arbitrage.** Begin at §1.1 Government Coupon Bonds (p. 50). Bond cash flows, discount factors, accrued interest and market conventions. Supports M1.
2. **Chapter 3, pp. 79–102 — Returns, Yields, Spreads, and P&L Attribution.** Begin at §3.2 Yield to Maturity (p. 82); later return to P&L attribution. Supports M1/M2 and research P&L.
3. **Chapter 4, pp. 103–134 — DV01, Duration, and Convexity.** Focus on §4.7 Yield-Based DV01, Duration, and Convexity (p. 119) after grasping the chapter's rate sensitivity concepts. Supports M2.
4. **Chapter 2, pp. 65–78 — Swap, Spot, and Forward Rates.** Distinguish zero, par and forward rates. Supports M3.
5. **Chapter 5, pp. 135–152 — Key-Rate, Partial, and Forward-Bucket '01s and Durations.** Multi-rate risk and hedging, when simple single-rate exposure is sound. Supports M4.
6. **Chapter 6, pp. 153–176 — Regression Hedging and Principal Component Analysis.** Optional conceptual background on correlated curve moves and empirical hedging; **PCA is not required** and no ML model is part of this project.

## Later only / outside the 8-week core

- Chapters 7–9 — term-structure and short-rate models. These are **not needed** to deliver a tested non-ML 2Y/10Y curve-risk project.
- Chapter 10 — repo and financing, useful background if refining carrying costs.
- Chapter 11 — Treasury note/bond futures, only if the instrument scope changes.

## Session 1 reading/implementation bridge

Read Chapter 1 pp. 49–53, then write the coupon-cash-flow schedule by hand for a $100, 4% coupon, 2-year bond paying semiannually. Derive the present-value sum with y=5% nominal annual, semiannual compounding. When that result is independently checked (~$98.119), implement the function in C++20.

The book's market examples are historical and the text may use simplifying assumptions; always state which assumptions the code adopts.
