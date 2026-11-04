# 06 — Test plan and acceptance criteria for the MVP

All financial calculations must be validated by **independent benchmark numbers or logically testable identities**, not only by reusing the same production function in expected values.

| Phase / proposed test | Independent test | Expected |
|---|---|---|
| 1 / `test_bond.cpp` | $100 face, 4% coupon, 2Y, semiannual | Four payments: 2, 2, 2, 102 |
| 1 / `test_pricing.cpp` | Same bond, 5% nominal YTM | **$98.1190129**, tolerance 1e-7 USD |
| 1 / `test_pricing.cpp` | Same bond, 4% nominal YTM | **$100.00** |
| 1 / `test_pricing.cpp` | Coupon 0, same YTM/frequency | **$100 / 1.025^4 ≈ $90.59506448** |
| 1 / `test_pricing.cpp` | Yield increases | Bond price falls |
| 2 / `test_pricing.cpp` | Solve YTM given price ≈ $98.1190129 | Approximately 5%; documented tolerance |
| 2 / `test_pricing.cpp` | Invalid price or nonbracketing interval | Explicit error |
| 3 / `test_risk.cpp` | Analytic modified duration and central bump | Agree within documented tolerance |
| 3 / `test_risk.cpp` | DV01 finite differences | Positive magnitude and correct $/bp unit |
| 3 / `test_risk.cpp` | Convexity test | Duration + convexity approximation improves for modest shocks in benchmark |
| 4 / `test_trade.cpp` | Equal absolute leg dollar DV01 | Signed net first-order parallel DV01 ≈ 0 |
| 5 / `test_scenarios.cpp` | +10/+10bp | Approx P&L ≈ 0 for DV01-balanced trade |
| 5 / `test_scenarios.cpp` | +5/+15bp | Positive steepener first-order P&L |
| 5 / `test_scenarios.cpp` | +15/+5bp | Negative steepener first-order P&L |
| 5 / `test_scenarios.cpp` | Exact repricing of both bonds | Calculate residual P&L even under parallel bump |

**Illustrative-slide checks (separate from actual model DV01):** $190/bp per $1m 2Y, $760/bp per $1m 10Y, long $4m 2Y / short $1m 10Y yields first-order $0, +$7,600, -$7,600 across the three scenarios. This is a test of the **hedge/P&L sign arithmetic**, not a market-data or exact-PV result.

## Test engineering rules

- Use `std::vector` for cash flows, doubles for pricing, sensible `std::size_t` indexing, and explicit input validation.
- Compare floating-point values with **absolute or relative tolerances**, never exact equality except discrete identities.
- Include error checks: nonpositive face, nonpositive coupon frequency, unsupported irregular maturity, invalid compounding base, unreachable YTM bracket, NaNs if rejected.
- Check notional scaling (price per $100 face versus face positions in millions); verify long/short signs.
- Register tests in **CMake/CTest** as each phase becomes implementable.
- For each passing phase record the test command and commit SHA in [../PROJECT_STATUS.md](../PROJECT_STATUS.md).

## Clean checkout release gate

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/treasury_curve_cli
```

The commands describe the **target**. The existing scaffold has **no financial tests registered**; it is not acceptable to claim a completed regression suite until test targets exist and pass.

## Optional add-on validation

If a zero-rate curve is added, verify discount-factor conversions and coupon-date PV separately. If a descriptive historical series is added, verify rate units, spread-basis-point conversion, date order and missing tenors. **Neither is required to pass the MVP release gate.**
