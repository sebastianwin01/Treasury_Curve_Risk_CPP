# 12 — Start here: Phase 1 (first focused implementation session)

## Objective

Implement **only the first small slice** of the project: $100-face regular fixed-coupon bond cash flows and a one-YTM price function. Do not create the other four module source files yet.

## Step 1 — finance first (15–25 minutes)

Use $100 face, 4% annual coupon, two years to maturity, 2 coupon payments/year.

- Coupon = `100 * 0.04 / 2 = $2`.
- Four future payments = `2, 2, 2, 102`.
- At nominal annual YTM 5% and semiannual compounding, periodic yield is `0.05/2 = 0.025`.
- Calculate `P = 2/1.025 + 2/1.025^2 + 2/1.025^3 + 102/1.025^4 ≈ 98.1190129`.
- At YTM 4%, confirm the price is $100.

Write these calculations before C++.

## Step 2 — create exactly these files

```text
include/treasury/Bond.h
src/Bond.cpp
include/treasury/BondPricing.h
src/BondPricing.cpp
tests/test_bond.cpp
tests/test_pricing.cpp
```

Recommended API in [11_technical_architecture.md](11_technical_architecture.md). First make the program compile and cash-flow tests pass; then implement pricing. Use `std::vector<CashFlow>`, not raw `new[]`/`delete[]`.

## Step 3 — connect to CMake

Keep the current `treasury_curve_cli` target. Add the implemented `Bond.cpp` and `BondPricing.cpp` to its build (or a small reusable `treasury_core` library), then register tests with CTest **only after the test files exist**.

## Step 4 — pass independent tests

- Cash-flow count is four; amounts are exactly the four expected dollar payments (within chosen tolerance).
- `priceFromYTM(bond, 0.05)` ≈ 98.1190129.
- `priceFromYTM(bond, 0.04)` ≈ 100.
- A zero-coupon $100 bond at 5% YTM prices to `100 / (1.025)^4`.
- Invalid frequency, face amount, irregular period count or compounding base produce clear errors.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

These test commands are the planned Phase 1 gate; they do **not yet** signify a passing test suite.

## Step 5 — record and push

After tests actually pass, record test commands and results in `PROJECT_STATUS.md`, then push your commit.

Example commit message: `Implement and validate regular-coupon bond pricing`.

## Next phase

Only after Phase 1 passes, implement bisection in `BondPricing.cpp`. There is **no need** to create a yield curve, funding model or backtester to start learning the core finance.
