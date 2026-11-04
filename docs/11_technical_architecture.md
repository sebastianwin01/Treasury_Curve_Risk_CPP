# Treasury Yield Curve Trading and Risk Analysis in C++

## Complete proposed architecture and file-by-file blueprint

**Status:** architecture proposal, **not an implementation**. Existing files have been checked against the GitHub `main` branch on November 4, 2026. This document does not create or modify any files in GitHub.

**Research goal:** price fixed-coupon bonds; solve YTM; compute duration, convexity and DV01; construct DV01-balanced 2Y–10Y steepener/flattener positions; test scenarios; and conditionally evaluate **synthetic historical** P&L. **No ML, Markov switching, stochastic forecasting or live trading.**

**Legend:** `[E]` already exists in the repository; `[N]` new file proposed; `[O]` optional/conditional (create only if justified). Directories without files are not tracked by Git; `README.md` placeholders preserve them.

## A. Full target directory tree

```text
Treasury_Curve_Risk_CPP/
├── README.md                                      [E] Project overview / eight phases
├── PROJECT_STATUS.md                              [E] Evidence-based milestones
├── CMakeLists.txt                                  [E] Extend with library + CTest
├── .gitignore                                     [E] Generated / private-file exclusions
├── project_manifest.json                          [E] Project metadata and scope
├── docs/
│   ├── 01_project_charter.md                       [E] Objective, scope, outcomes
│   ├── 02_math_and_conventions.md                  [E] Equations, units, calendar rules
│   ├── 03_trading_strategy.md                      [E] 2Y–10Y trade signs/hedging
│   ├── 04_data_plan.md                             [E] Yield sources, format, checks
│   ├── 05_backtest_methodology.md                  [E] Synthetic bonds and cash accounting
│   ├── 06_validation_plan.md                       [E] Benchmarks / tolerances
│   ├── 07_eight_week_roadmap.md                    [E] Eight-phase plan
│   ├── 08_reading_map.md                           [E] Relevant textbook sections
│   ├── 09_decision_log.md                          [E] Confirmed vs proposed decisions
│   ├── 10_final_report_outline.md                 [E] Report/presentation headings
│   └── 11_technical_architecture.md               [N] This architecture document
├── include/
│   ├── README.md                                   [E] Existing placeholder
│   └── treasury/
│       ├── BondTypes.h                             [N] BondSpec, CashFlow
│       ├── BondCashFlows.h                         [N] Cash-flow generator
│       ├── BondPricing.h                           [N] YTM and curve pricing
│       ├── YieldSolver.h                           [N] Bisection YTM solver
│       ├── BondRisk.h                              [N] Duration, convexity, DV01
│       ├── YieldCurve.h                            [N] Term structure / discounting
│       ├── Position.h                              [N] Signed position representation
│       ├── TradeSizing.h                           [N] 2Y–10Y DV01-neutral hedging
│       ├── CurveShock.h                            [N] Rate-shock definitions
│       ├── ScenarioAnalysis.h                      [N] Scenario P&L results
│       ├── MarketData.h                            [N] Par vs zero curve data types
│       ├── DatedBond.h                             [O] Dated synthetic bond contracts
│       ├── Financing.h                             [O] Funding / short-cost parameters
│       ├── CashLedger.h                            [O] Coupon/trade/collateral cash ledger
│       ├── Backtester.h                            [O] Historical synthetic simulation
│       ├── Performance.h                           [O] Drawdown/P&L summary
│       └── ResultWriter.h                          [O] CSV outputs
├── src/
│   ├── main.cpp                                    [E] CLI scaffold; wire features later
│   ├── BondCashFlows.cpp                           [N]
│   ├── BondPricing.cpp                             [N]
│   ├── YieldSolver.cpp                             [N]
│   ├── BondRisk.cpp                                [N]
│   ├── YieldCurve.cpp                              [N]
│   ├── TradeSizing.cpp                             [N]
│   ├── CurveShock.cpp                              [N]
│   ├── ScenarioAnalysis.cpp                        [N]
│   ├── MarketData.cpp                              [N]
│   ├── DatedBond.cpp                               [O]
│   ├── Financing.cpp                               [O]
│   ├── CashLedger.cpp                              [O]
│   ├── Backtester.cpp                              [O]
│   ├── Performance.cpp                             [O]
│   └── ResultWriter.cpp                            [O]
├── tests/
│   ├── README.md                                   [E] Existing placeholder
│   ├── test_bond_pricing.cpp                       [N] Cash flows, price, par
│   ├── test_yield_solver.cpp                       [N] YTM convergence and failure cases
│   ├── test_bond_risk.cpp                          [N] Analytic versus finite difference
│   ├── test_yield_curve.cpp                        [N] Discounts / interpolation
│   ├── test_trade_sizing.cpp                       [N] DV01 hedge ratios
│   ├── test_scenario_analysis.cpp                  [N] Signs / full repricing
│   ├── test_market_data.cpp                        [N] CSV units, dates, missing values
│   ├── test_dated_bond.cpp                         [O] Aging / coupon dates
│   ├── test_cash_ledger.cpp                        [O] Cash flow reconciliation
│   ├── test_financing.cpp                          [O] Funding and transaction expenses
│   └── test_backtester.cpp                         [O] Synthetic valuation consistency
├── data/
│   ├── sample/
│   │   ├── known_price_cases.csv                   [E] Initial bond-pricing benchmarks
│   │   ├── sample_zero_curve.csv                   [N] Independent curve benchmark inputs
│   │   ├── sample_2y10y_shocks.csv                 [N] Parallel/steepener/flattener cases
│   │   └── synthetic_history_fixture.csv           [O] Small known-result backtest fixture
│   ├── raw/
│   │   └── README.md                               [E] Source and acquisition policy
│   └── processed/
│       └── README.md                               [E] Normalized and verified outputs
├── python/
│   ├── README.md                                   [E] Optional non-ML tooling
│   ├── prepare_treasury_data.py                    [O] Date/unit cleaning for CSV
│   └── plot_results.py                             [O] Charts from produced results
└── results/
    ├── README.md                                   [E] Generated-file policy
    ├── scenario_results.csv                        [O] Generated, usually ignored
    ├── synthetic_backtest.csv                       [O] Generated if feasible
    └── figures/                                     [O] Generated charts, not source
```

## B. Design guidelines

1. Use namespace `treasury` and `std::vector`, `std::string`, and value types. Do **not** manually own heap arrays for pricing modules. C++ standard types manage their own memory; no explicit Rule of Five is needed for these records.
2. **Start with pure functions** and simple `struct` values. Only a component that actually owns state (e.g. `YieldCurve`, `CashLedger`) needs to be a class. Do not build a hierarchy of abstract bonds at Milestone 1.
3. `BondSpec.faceValueUsd` is a **quotation unit** (e.g. $100); `Position.faceAmountUsd` is the actual modeled investment size. Multiply bond-unit values by `faceAmountUsd / faceValueUsd` to obtain portfolio dollars.
4. Store yields internally as decimal annual rates (`0.05`, not `5.0`), changes in basis points only when explicitly named `...Bp`, cash in USD and duration in years.
5. A `YieldCurve` must contain **zero-rate/discount-compatible information**, not unconverted Treasury constant-maturity **par yields**. Market data types should enforce this distinction.
6. All C++ function declarations belong in `.h`; definitions belong in matching `.cpp` files. Header-only small structs and enums are fine.
7. Document and test numerical inputs and invalid cases; check compounding bases, strictly increasing curve tenors and well-formed coupon periods.
8. Any historical P&L without actual security-level executed prices is labeled **synthetic** and is conditional on sound curve inputs and explicit cash/funding conventions.

## C. C++ header-by-header API blueprint

The signatures below are **proposed interfaces**, not compiling, implemented source files. Include the relevant standard headers in each header (`<vector>`, `<string>`, `<chrono>`, etc.). Name changes are okay if tracked consistently in documentation, source and tests.

### C1. `include/treasury/BondTypes.h` — Phase 1

**Purpose:** Fundamental value types shared across pricing modules.

```cpp
namespace treasury {
struct BondSpec {
    double faceValueUsd{100.0};        // Quote unit, not trade notional
    double annualCouponRate{0.04};     // Decimal annual coupon
    double maturityYears{2.0};        // Regular whole coupon periods at first
    int couponsPerYear{2};            // e.g. semiannual = 2
};
struct CashFlow {
    double timeYears{};               // Years from valuation to payment
    double amountUsd{};               // Per BondSpec.faceValueUsd
};
}
```

**Rules:** validate face > 0, coupon frequency > 0, maturity > 0, finite inputs; first version restricts `maturityYears * couponsPerYear` to an integer number of regular periods. No methods or owned memory needed.

### C2. `BondCashFlows.h` / `BondCashFlows.cpp` — Phase 1

```cpp
std::vector<CashFlow> generateRegularCashFlows(const BondSpec& bond);
void validateBondSpec(const BondSpec& bond);
```

**Algorithm:** period `k=1..N` has `timeYears=k/m`; coupon `F*c/m`; final payment includes `F`. In the first sample this produces `(2, 2, 2, 102)` at `(0.5, 1.0, 1.5, 2.0)` years. No class members.

### C3. `BondPricing.h` / `BondPricing.cpp` — Phases 1 and 3

```cpp
double priceFromYTM(const BondSpec& bond, double ytmDecimal);
double priceCashFlowsFromYTM(const std::vector<CashFlow>& cashFlows,
                             double ytmDecimal, int couponsPerYear);
// Add only after YieldCurve is available (Phase 3):
double priceFromCurve(const BondSpec& bond, const YieldCurve& zeroCurve);
```

**Methods:** pure functions; no stateful class. **Formula:** `P = sum CF_k / pow(1+y/m, k)`. For curve pricing, `P = sum CF_k * curve.discountFactor(timeYears)`. Both return USD *per bond quotation unit*. Check that `1+y/m > 0`; historical dated cash flows will need a separate dated schedule.

### C4. `YieldSolver.h` / `YieldSolver.cpp` — Phase 2

```cpp
struct YieldSolverSettings {
    double lowerYieldDecimal{-0.10};  // Chosen bracket, not universal
    double upperYieldDecimal{0.50};   // Chosen bracket, not universal
    double priceToleranceUsd{1e-10};
    double yieldTolerance{1e-12};
    int maxIterations{200};
};
struct YieldSolveResult {
    double yieldDecimal{};
    double priceResidualUsd{};
    int iterations{};
};
YieldSolveResult solveYTM(const BondSpec& bond, double observedPriceUsd,
                          const YieldSolverSettings& settings);
```

**Algorithm:** bisection of `priceFromYTM(bond,y) - observedPriceUsd`; require positive observed price, valid YTM domain and a sign-changing bracket; return convergence evidence or explicitly throw on failure. These bracket defaults are examples only.

### C5. `BondRisk.h` / `BondRisk.cpp` — Phase 2

```cpp
struct BondRiskMetrics {
    double macaulayDurationYears{};
    double modifiedDurationYears{};
    double convexityYearsSquared{};
    double dv01UsdPerBp{};              // Per bond quote unit
};
BondRiskMetrics calculateBondRisk(const BondSpec& bond, double ytmDecimal);
double macaulayDuration(const BondSpec& bond, double ytmDecimal);
double modifiedDuration(const BondSpec& bond, double ytmDecimal);
double convexity(const BondSpec& bond, double ytmDecimal);
double dv01(const BondSpec& bond, double ytmDecimal);
double dv01FiniteDifference(const BondSpec& bond, double ytmDecimal,
                            double yieldBumpDecimal = 0.0001);
double approxPriceChange(const BondSpec& bond, double ytmDecimal,
                         double deltaYieldDecimal, bool useConvexity);
```

**Checks:** positive conventional long-bond DV01, duration consistent with maturity, derivative/finite-difference agreement, plus/minus one-basis-point and larger reprice discrepancies. State which convexity and YTM convention is used.

### C6. `YieldCurve.h` / `YieldCurve.cpp` — Phase 3

```cpp
struct ZeroCurveNode {
    double maturityYears{};
    double zeroRateDecimal{};       // Annual rate, convention stated below
};
enum class ZeroCompounding { Continuous, Semiannual };
enum class CurveInterpolation { LinearZeroRate, LinearLogDiscount };
class YieldCurve {
private:
    std::vector<ZeroCurveNode> nodes_;
    ZeroCompounding compounding_;
    CurveInterpolation interpolation_;
public:
    YieldCurve(std::vector<ZeroCurveNode> nodes,
               ZeroCompounding compounding,
               CurveInterpolation interpolation);
    double zeroRate(double maturityYears) const;
    double discountFactor(double maturityYears) const;
    const std::vector<ZeroCurveNode>& nodes() const noexcept;
};
```

**Invariant:** sorted unique positive tenors, finite rates, consistent compounding; explicit behavior at `t=0` (`D(0)=1`) and outside observed range (error or documented extrapolation). **A par-yield data series must not be used to construct this class directly.**

### C7. `Position.h` — Phase 4

```cpp
enum class PositionSide { Long = 1, Short = -1 };
struct Position {
    BondSpec bond{};
    double faceAmountUsd{};         // Total notional; >=0
    PositionSide side{PositionSide::Long};
};
```

**Behavior:** signed modeled bond value = side × `(faceAmountUsd / bond.faceValueUsd)` × quote-unit price. Keep the sign separate from nonnegative notional.

### C8. `TradeSizing.h` / `TradeSizing.cpp` — Phase 4

```cpp
struct CurveTrade {
    Position twoYear;
    Position tenYear;
};
struct TradeExposure {
    double dv01TwoYearUsdPerBp{};   // Signed dollar exposure
    double dv01TenYearUsdPerBp{};
    double netParallelDv01UsdPerBp{};
};
double hedgeFaceAmountUsd(double anchorFaceAmountUsd,
                          double anchorDv01PerQuoteUnit, double anchorQuoteFaceUsd,
                          double hedgeDv01PerQuoteUnit, double hedgeQuoteFaceUsd);
CurveTrade makeDv01BalancedSteepener(const BondSpec& twoYear,
                                     const BondSpec& tenYear,
                                     double twoYearYTM, double tenYearYTM,
                                     double chosenTwoYearFaceUsd);
CurveTrade makeDv01BalancedFlattener(const BondSpec& twoYear,
                                     const BondSpec& tenYear,
                                     double twoYearYTM, double tenYearYTM,
                                     double chosenTwoYearFaceUsd);
TradeExposure calculateTradeExposure(const CurveTrade& trade,
                                     double twoYearYTM, double tenYearYTM);
```

**Note:** hedge sizing must normalize by each bond's quote face amount; the illustrative $4m/$1m ratio only applies to the stated illustrative DV01s. Use bond-specific model DV01s for actual model sizing.

### C9. `CurveShock.h` / `CurveShock.cpp` — Phase 5

```cpp
struct ShockNode {
    double maturityYears{};
    double shiftBp{};
};
struct CurveShock {
    std::string name;
    std::vector<ShockNode> shiftsBp;
};
YieldCurve applyCurveShock(const YieldCurve& baseCurve,
                           const CurveShock& shock);
```

**Behavior:** interpolate shock shifts with an explicit maturity/boundary rule; apply **rate shifts** consistently with the underlying zero curve, not blindly as separate coupon-bond YTM changes. Simple example scenarios: `+10/+10 bp` parallel, `+5/+15 bp` steepening, `+15/+5 bp` flattening at 2Y/10Y.

### C10. `ScenarioAnalysis.h` / `ScenarioAnalysis.cpp` — Phase 5

```cpp
struct ScenarioResult {
    std::string name;
    double firstOrderPnlUsd{};
    double durationConvexityPnlUsd{};
    double fullRepricePnlUsd{};
    double approximationErrorUsd{};
};
ScenarioResult evaluateScenario(const CurveTrade& trade,
                                const YieldCurve& baseCurve,
                                const CurveShock& shock);
```

**Behavior:** model starting/ending curve prices for each leg; compare DV01/convexity approximations and full repricing, with explicit signs and common quote units. Phase 5 can use a simplified two-YTM scenario first; a full curve-shock test is the stronger final implementation.

### C11. `MarketData.h` / `MarketData.cpp` — Phase 6 (feasibility check in Phase 3)

```cpp
using Date = std::chrono::sys_days;
struct ParYieldObservation {
    Date date{};
    double twoYearParYieldDecimal{};
    double tenYearParYieldDecimal{};
};
struct ZeroCurveSnapshot {
    Date date{};
    std::vector<ZeroCurveNode> nodes;
};
std::vector<ParYieldObservation> loadParYieldsCsv(const std::string& path);
std::vector<ZeroCurveSnapshot> loadZeroCurvesCsv(const std::string& path);
```

**Checks:** unique dates/tenors, chronological ordering, missing values, correct percentage-to-decimal conversion, provenance metadata, consistent rate types. A par observation must never be passed as a zero curve without an explicit verified conversion/bootstrapping method.

### C12. `DatedBond.h` / `DatedBond.cpp` — Conditional Phase 7

```cpp
struct DatedBond {
    std::string instrumentId;
    double faceValueUsd{100.0};
    double annualCouponRate{};
    std::vector<Date> couponDates; // Explicit; increasing and unambiguous
    Date maturityDate{};
};
struct DatedCashFlow {
    Date paymentDate{};
    double amountUsd{};
};
std::vector<DatedCashFlow> remainingCashFlows(const DatedBond& bond, Date asOf);
std::vector<DatedCashFlow> paymentsBetween(const DatedBond& bond,
                                            Date fromExclusive, Date toInclusive);
double dirtyPriceFromCurve(const DatedBond& bond, Date asOf,
                           const YieldCurve& valuationCurve);
```

**Why separate:** the early `BondSpec` assumes regular equal periods; historically aging bonds have real payment dates. Define year fractions, settlement and cash treatment **before** applying dated pricing. Never reset coupon or maturity of an existing held bond when time advances.

### C13. `Financing.h` / `Financing.cpp` — Conditional Phase 7

```cpp
struct FinancingAssumptions {
    double annualLongFundingRate{};
    double annualShortBorrowRate{};
    double annualShortProceedsRebateRate{};
    double executionCostBp{};
    double longHaircutFraction{};
};
double accrueFundingCost(double fundedUsd, double annualRate,
                         double yearFraction);
double executionCostUsd(double tradedMarketValueUsd, double costBp);
```

**Rules:** never assume free short-sale proceeds, no short rebate or zero costs without labeling assumptions. Costs are model parameters for sensitivity analysis—not observed trades. Some rates may be signed; document conventions.

### C14. `CashLedger.h` / `CashLedger.cpp` — Conditional Phase 7

```cpp
enum class LedgerKind { Purchase, Sale, Coupon, Principal,
                        Funding, ShortBorrow, Rebate, TransactionCost };
struct LedgerEntry {
    Date date{};
    LedgerKind kind{};
    double amountUsd{};           // Signed cash change
    std::string description;
};
class CashLedger {
private:
    std::vector<LedgerEntry> entries_;
    double cashBalanceUsd_{0.0};
public:
    void post(const LedgerEntry& entry);
    double cashBalanceUsd() const noexcept;
    const std::vector<LedgerEntry>& entries() const noexcept;
};
```

**Checks:** coupon paid to long/owed on short, bond principal at redemption, costs and balances; reconcile portfolio changes with cash ledger. Realistic collateral accounts may require additional state; don't silently treat collateral as spendable cash.

### C15. `Backtester.h` / `Backtester.cpp` — Conditional Phase 7

```cpp
struct BacktestConfig {
    Date startDate{};
    Date endDate{};
    int holdDays{};                  // Proposed; decision not yet locked
    double initialTwoYearFaceUsd{};
    FinancingAssumptions financing{};
};
struct BacktestRow {
    Date date{};
    double portfolioValueUsd{};
    double cashBalanceUsd{};
    double netDv01UsdPerBp{};
    double grossPnlUsd{};
    double netPnlUsd{};
};
struct BacktestResult {
    std::vector<BacktestRow> rows;
    std::string assumptionSummary;
};
BacktestResult runSyntheticBacktest(
    const std::vector<ZeroCurveSnapshot>& datedCurves,
    const BacktestConfig& config);
```

**Behavior:** initialize **fixed coupon** synthetic bonds, hold/age them, accrue time and coupons, reprice at successive dates, post funding and transaction costs, optionally rebalance under a prespecified rule, and export a reconciled cash/P&L ledger. Only implement when data and conventions are sufficiently reliable.

### C16. `Performance.h` / `Performance.cpp` — Conditional Phase 7

```cpp
struct PerformanceSummary {
    double cumulativeGrossPnlUsd{};
    double cumulativeNetPnlUsd{};
    double maxDrawdownUsd{};
};
PerformanceSummary summarizePerformance(const BacktestResult& result);
```

**Rules:** report dollar P&L first; percent returns, Sharpe and capital-based drawdown require clearly defined invested capital/financing and sampling assumptions.

### C17. `ResultWriter.h` / `ResultWriter.cpp` — Phase 8, conditional on output

```cpp
void writeScenarioResultsCsv(const std::string& path,
                             const std::vector<ScenarioResult>& results);
void writeBacktestResultsCsv(const std::string& path,
                             const BacktestResult& results);
```

**Behavior:** stable, documented column names and financial units, reproducible outputs, no embedded magic data.

### `src/main.cpp` — all stages

**Should include:** parse a few CLI arguments or call a controlled demonstration; use library functions to run benchmark pricing and scenarios. **Should not include:** pricing formulas, numerical solvers, CSV parsing or backtest algorithms. As of the reviewed GitHub `main`, `main.cpp` only prints the starter banner.

## D. Matching `.cpp` implementation responsibilities

Every non-header-only component has a matching `.cpp`. This is the submap of source implementations, not additional classes:

| Implementation | Contains | Phase |
|---|---|---|
| `BondCashFlows.cpp` | Input validation; regular coupon generation; final principal | 1 |
| `BondPricing.cpp` | Discounting loops; YTM and eventually curve valuation | 1, 3 |
| `YieldSolver.cpp` | Bisection loop, bracket validation, residual/iteration stopping | 2 |
| `BondRisk.cpp` | Duration, convexity, DV01 and price-shock approximation | 2 |
| `YieldCurve.cpp` | Node validation, interpolation, zero-to-discount conversion | 3 |
| `TradeSizing.cpp` | Dollar-DV01 scaling, two-leg hedge ratio, exposure summaries | 4 |
| `CurveShock.cpp` | Define and apply maturity-dependent rate shocks | 5 |
| `ScenarioAnalysis.cpp` | Baseline/repriced position values and error attribution | 5 |
| `MarketData.cpp` | CSV parsing, date normalization, data-quality checks | 6 |
| `DatedBond.cpp` | Historical coupon schedule/aging and dirty-price valuation | 7, conditional |
| `Financing.cpp` | Cost accrual and execution assumptions | 7, conditional |
| `CashLedger.cpp` | Record signed cash transactions, balance and reconciliation | 7, conditional |
| `Backtester.cpp` | Daily synthetic positions and backtest lifecycle | 7, conditional |
| `Performance.cpp` | Meaningful P&L aggregates and drawdown | 7, conditional |
| `ResultWriter.cpp` | Write clearly labeled reproducible CSV results | 8, conditional |

## E. Automated test file map

| Test file | Required checks |
|---|---|
| `test_bond_pricing.cpp` | Four cash flows `(2,2,2,102)`; 5% YTM ~$98.1190129; 4% YTM $100; zero coupon; yield monotonicity; invalid inputs |
| `test_yield_solver.cpp` | Recover 5% from $98.1190129 and 4% from $100; bracket failures; iteration and residual tolerances |
| `test_bond_risk.cpp` | Analytic versus central-difference duration/DV01/convexity; ±1bp and ±100bp; sign and unit checks |
| `test_yield_curve.cpp` | Known discount factors; known PV; compounding choice; interpolation; duplicate tenors; missing bounds |
| `test_trade_sizing.cpp` | Correct long/short signs and common-face scaling; matched DV01; zero/invalid input checks |
| `test_scenario_analysis.cpp` | Parallel +10/+10 bp ≈ zero first-order P&L; steepening +5/+15 bp positive for steepener; flattening sign reversed; full repricing residual |
| `test_market_data.cpp` | Percent→decimal; date sorting; duplicates; missing 2Y/10Y pairs; par/zero type distinction |
| `test_dated_bond.cpp` | Coupon due dates, aging, final principal, coupon amount unchanged after entry |
| `test_financing.cpp` | Rate/elapsed-time conventions; long funding, short costs, transaction fees |
| `test_cash_ledger.cpp` | Signed coupons/cash, short coupon payments, traded notionals, ledger reconciliation |
| `test_backtester.cpp` | A small hand-computed synthetic history, no-lookahead, rebalancing entries, gross/net P&L, missing date behavior |

Use **CTest** and simple `assert`/custom checks to start. Add an external testing framework only if it genuinely helps. A failing test must return a nonzero process exit status.

## F. All existing documentation and repository-root files

| File | What it should include / maintain |
|---|---|
| `README.md` | Research question, non-ML scope, eight phases, install/build commands, actual vs proposed status, synthetic-only limitations, links |
| `PROJECT_STATUS.md` | Per-phase status, evidence links/commit IDs, benchmark acceptance and current next task; no speculative completion claims |
| `CMakeLists.txt` | C++20 standard, warning flags, static/core library sources, `treasury_curve_cli`, testing with CTest, optional output paths |
| `.gitignore` | IDE/build artifacts and cache files; raw/processed data or generated results as appropriate; no API keys or private credentials |
| `project_manifest.json` | Machine-readable title, research question, scope, expected modules and honest current stage |
| `docs/01_project_charter.md` | Scope, collaborators/roles once agreed, goals, constraints, and definition of done |
| `docs/02_math_and_conventions.md` | Pricing and YTM equations; discount-factor conventions; risk derivatives; currency/time/rate units; later actual-date conventions |
| `docs/03_trading_strategy.md` | Definition of steepening/flattening; DV01-neutral ratio; scenario examples; caveats |
| `docs/04_data_plan.md` | Source provenance, historical observations, par-vs-zero distinction, schema, missing-data and date rules |
| `docs/05_backtest_methodology.md` | Synthetic contracts, fixed coupons, aging, clean/dirty and coupon ledger, funding/collateral/transaction costs, backtest biases and go/no-go |
| `docs/06_validation_plan.md` | Exact numerical acceptance tests, independent benchmarks, tolerances, edge cases and cash-ledger reconciliation |
| `docs/07_eight_week_roadmap.md` | Tasks, week-by-week phases, review gates, and deliverable dependencies |
| `docs/08_reading_map.md` | Tuckman chapters 1–6 mapped to the implementation milestones |
| `docs/09_decision_log.md` | Confirmed vs proposed conventions; open decisions and reasons for each change |
| `docs/10_final_report_outline.md` | Abstract, theory, methodology, implementation, tested results, limitations and conclusions |
| `docs/11_technical_architecture.md` | **New:** target file structure, types, interfaces, dependencies and test mapping (this document) |
| `include/README.md` | Placeholder and notes explaining public API and namespace; it may be removed when `.h` files exist |
| `tests/README.md` | How to run tests and interpret failures; file-to-milestone coverage |
| `python/README.md` | Scope of non-ML helper scripts, requirements and usage |
| `data/raw/README.md` | Raw data source, license/provenance, ignored files and acquisition instructions |
| `data/processed/README.md` | Normalized schema, transforms, missing-value policy, regeneration steps |
| `results/README.md` | Output schema, regeneration command, synthetic disclaimer; generated files are normally ignored |

## G. Data file map

| File | Suggested columns | Key checks |
|---|---|---|
| `data/sample/known_price_cases.csv` `[E]` | case_id, face_value_usd, annual_coupon_rate_decimal, maturity_years, payments_per_year, ytm_nominal_annual_decimal, expected_price_usd, description | Keep independent $98.119/$100/zero cases; do not alter benchmarks to fit buggy code |
| `data/sample/sample_zero_curve.csv` `[N]` | maturity_years, zero_rate_decimal, compounding | Clearly declared compounding, nonduplicate tenors |
| `data/sample/sample_2y10y_shocks.csv` `[N]` | scenario, shift_2y_bp, shift_10y_bp, expected_first_order_pnl_usd | Include parallel and two opposing slope shocks |
| `data/sample/synthetic_history_fixture.csv` `[O]` | valuation_date, maturity_years, zero_rate_decimal, plus documented fixture metadata | Manually reconcile tiny history before broad data |
| `data/raw/*` | Original source files, ignored unless permitted | Never overwrite originals silently |
| `data/processed/*` | Standardized date, rate_type, maturity_years, annual_rate_decimal, compounding, quality_flag | Reject missing/duplicate records; keep provenance |
| `results/scenario_results.csv` `[O]` | scenario, approx_pnl_usd, full_reprice_pnl_usd, error_usd | Clear scenario/synthetic labels |
| `results/synthetic_backtest.csv` `[O]` | date, both leg notionals, signed DV01, cash, coupon, financing, gross/net P&L | Reconcile inventory and cash; labeled synthetic |

## H. CMake and dependency map

```text
main.cpp
  └── treasury_core (CMake library)
        ├── BondTypes.h
        ├── BondCashFlows.cpp
        ├── BondPricing.cpp
        ├── YieldSolver.cpp        --> priceFromYTM
        ├── BondRisk.cpp           --> priceFromYTM + bond cash flows
        ├── YieldCurve.cpp         --> nodes / interpolation
        ├── TradeSizing.cpp        --> BondRisk + Position
        ├── CurveShock.cpp         --> YieldCurve
        ├── ScenarioAnalysis.cpp   --> TradeSizing + CurveShock + BondPricing
        ├── MarketData.cpp         --> CSV + rate type definitions
        └── Optional dated simulator
             ├── DatedBond.cpp
             ├── Financing.cpp
             ├── CashLedger.cpp
             ├── Backtester.cpp
             ├── Performance.cpp
             └── ResultWriter.cpp

tests/test_*.cpp -> treasury_core -> CTest
python/*.py      -> input cleaning / result plots only
```

CMake **target intent** after implementation: `add_library(treasury_core ...)`, `target_include_directories(treasury_core PUBLIC include)`, `target_link_libraries(treasury_curve_cli PRIVATE treasury_core)`, `include(CTest)`, `add_executable(test_bond_pricing ...)`, `add_test(...)`. Add source files **as they are written**, not all at once before the files exist.

## I. Build order (most important practical guidance)

| Phase | Create these files **when you reach it** | Do not do yet |
|---|---|---|
| 1 | `BondTypes.h`, `BondCashFlows.h/.cpp`, `BondPricing.h/.cpp`, `test_bond_pricing.cpp` | Yield curve/backtest classes |
| 2 | `YieldSolver.h/.cpp`, `BondRisk.h/.cpp`, corresponding tests | Historical finance mechanisms |
| 3 | `YieldCurve.h/.cpp`, `test_yield_curve.cpp`; investigate zero-curve data now | Assuming par yields are zeros |
| 4 | `Position.h`, `TradeSizing.h/.cpp`, `test_trade_sizing.cpp` | Predictive strategies |
| 5 | `CurveShock.h/.cpp`, `ScenarioAnalysis.h/.cpp`, scenario tests | Realized-return claims |
| 6 | `MarketData.h/.cpp`, `test_market_data.cpp`, optional CSV cleaning Python | Unverifiable historical valuation |
| 7 | **Only if defensible:** `DatedBond`, `Financing`, `CashLedger`, `Backtester`, `Performance` modules and tests | Pretend executable fills or live orders |
| 8 | `ResultWriter` if needed, plots, final report, full CTest run and README updates | New unvalidated features |

**Immediate coding target:** price the $100-face, 4%-coupon, 2-year semiannual bond at a 5% nominal annual YTM and obtain **$98.1190129**, then confirm par at 4%, monotonicity, zero-coupon pricing, and invalid-input behavior.
