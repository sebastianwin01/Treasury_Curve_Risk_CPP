# Planned C++ interfaces (no completed implementation)

Begin with free functions and simple standard-library containers. Once individual formulas are validated, consider interfaces such as:

- `CashFlow` (time in years and amount in dollars).
- `BondSpec` (face amount, coupon rate, payment frequency, maturity/date convention).
- `price_from_ytm(...)` and `ytm_from_price(...)`.
- `macaulay_duration(...)`, `modified_duration(...)`, `convexity(...)`, `dv01(...)`.
- `DiscountCurve` with explicit curve compounding and interpolation.
- `PortfolioLeg` and `CurveTrade` with signed notionals and instrument identifiers.

The sequence is pedagogical: derive and validate before designing abstractions. Do not introduce dynamic owning pointers for numeric arrays already handled by `std::vector`.
