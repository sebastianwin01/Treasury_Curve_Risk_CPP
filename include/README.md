# C++ headers — five-module MVP

Only add source interfaces when implementing their phases, and keep the CMake build working.

Required phase order:
1. `treasury/Bond.h` — fixed-coupon bond properties and regular cash flows.
2. `treasury/BondPricing.h` — YTM price and bisection solver.
3. `treasury/BondRisk.h` — duration, convexity, DV01.
4. `treasury/CurveTrade.h` — two signed legs and DV01 matching.
5. `treasury/ScenarioAnalysis.h` — immediate YTM shocks and P&L.

**Optional after MVP only:** `YieldCurve.h` and `MarketData.h`.

Use plain data structures and `std::vector`; no raw owning pointers are required. Names, member types and tests are in [docs/11_technical_architecture.md](../docs/11_technical_architecture.md). `include/treasury/README.md` reserves the directory without pretending classes exist.
