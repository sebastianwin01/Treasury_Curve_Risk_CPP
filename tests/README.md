# Tests — required for every implemented module

The MVP's planned test files are:

- `test_bond.cpp` — coupons, final principal, invalid specs.
- `test_pricing.cpp` — `P(5%) ≈ $98.1190129`, par and zero-coupon cases, bisection YTM.
- `test_risk.cpp` — analytic/finite-difference duration, convexity and DV01.
- `test_trade.cpp` — signed risk neutrality and notional units.
- `test_scenarios.cpp` — parallel, steepener and flattener signs; first-order versus exact repricing.

Implement and register each test with CTest **when its source module exists**. The current project is still a scaffold with **no confirmed passing financial tests**.

Tests must compare against independent numbers and formulas. See [docs/06_validation_plan.md](../docs/06_validation_plan.md).
