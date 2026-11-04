# Treasury public interfaces — planned, not yet coded

**Start with** `Bond.h` and `BondPricing.h`. Add the accompanying `src/Bond.cpp`, `src/BondPricing.cpp` and tests **in the same phase**, then advance to `BondRisk`, `CurveTrade` and `ScenarioAnalysis`.

No empty C++ headers are checked in just to fill the directory. The target class members and function signatures are defined in [docs/11_technical_architecture.md](../../docs/11_technical_architecture.md).

Zero-rate `YieldCurve` and descriptive `MarketData` are optional extensions after all MVP tests pass.
