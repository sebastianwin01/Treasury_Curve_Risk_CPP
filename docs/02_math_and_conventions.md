# 02 — Mathematical specification and conventions

This is a teaching-first specification. It defines the minimum equations before a general production system is considered.

## A. Fixed-rate bond cash flows

For **face value** F in dollars, **annual coupon rate** c (decimal), **coupon frequency** m payments/year and N total future *regular* periods:

- Each regular coupon equals `F * c / m` dollars.
- The last payment also includes principal `F`.
- At period k, cash flow is `CF_k = F*c/m + (k == N ? F : 0)`.

Assume equally spaced regular periods for M1. Real issuance dates, holidays, settlement, accrued interest and day-count conventions are future improvements; they must not be silently assumed away in a historical study.

## B. Single-YTM pricing

With nominal annual YTM y (decimal), compounded m times/year:

`P(y) = SUM_{k=1}^N CF_k / (1+y/m)^k`

Here P and CF are in dollars per specified F, y is annualized decimal, and the exponent k counts coupon periods. Require `(1+y/m) > 0` for this expression.

**Manual benchmark:** F=100, c=0.04, m=2, T=2 years, N=4, y=0.05; cash flows `(2,2,2,102)` and:

`P = 2/1.025 + 2/1.025^2 + 2/1.025^3 + 102/1.025^4 ≈ 98.1190129`

At y=0.04, P=100. For fixed positive cash flows, higher y yields lower P.

**Important distinction:** This one-rate pricing uses *bond YTM*, not a separate zero rate for each coupon date. Do not substitute a set of Treasury par yields for cash-flow-specific discount rates.

## C. Solve YTM from a known price

Given a strictly positive bond price P_market, solve `f(y) = P(y) - P_market = 0` via bisection on a bracket where f changes sign. Check interval validity, number of iterations and absolute price/yield tolerances. Report failure on invalid/unbracketed inputs.

## D. Curve discounting

For discount factor D(t_k) dimensionless at time t_k years:

`P_curve = SUM_{k=1}^N CF_k * D(t_k)`.

For **continuously compounded zero rate** z(t): `D(t) = exp(-z(t) * t)`.

Compounding conventions must be explicit; `exp(-z*t)` is not interchangeable with `(1+y/m)^(-m*t)` unless rates have been converted properly. Specify interpolation in **discount factors**, **log discount factors** or **zero rates**, and document its numerical consequences.

## E. Yield sensitivities (one-factor)

`D_mod = -(1/P) * dP/dy` in years for y expressed as decimal per year.

`DV01 = - (dP/dy) * 0.0001` in dollars per 1bp rise/fall magnitude, for a long position with conventional positive cash flows.

`C = (1/P) * d²P/dy²` (convexity, approximately years²).

Approximate price change for `Δy` (annual decimal) is:

`ΔP ≈ -P * D_mod * Δy + (P*C/2) * (Δy)^2`.

For a 1bp *finite-difference* parallel YTM bump (h=0.0001):

`DV01_fd ≈ [P(y-h)-P(y+h)]/2` (positive dollars/bp for a typical long positive-cash-flow bond).

Divide by 2—not by 2h—to get dollars for a 1bp move; dividing by 2h instead approximates `-dP/dy`, in dollars per unit of decimal annual yield. Do not confuse these units.

Verify finite differences across multiple h values to see convergence versus floating-point error. Distinguish market-value DV01 from per-$100-face DV01 by applying the chosen notional scaling.

## F. Macaulay duration

For a regular coupon bond with period times `t_k=k/m` years and periodic YTM discounting:

`D_mac = (1/P) * SUM_{k=1}^N t_k * CF_k / (1+y/m)^k`.

`D_mod = D_mac / (1+y/m)` under the same nominal YTM and coupon frequency.

## G. Risk beyond a single yield

Historical curve exposures are **not** fully characterized by parallel YTM shifts. Reprice a coupon bond by each dated curve discount factor. Later implement curve bumps, key-rate sensitivities and compare with a one-factor approximation; specify how the bumps interpolate through nearby maturities.

## H. Conventions checklist to fill in before historical backtesting

- Quote unit: yields expressed in decimal internally; display as percent/basis points.
- Valuation per $100 face vs. per $1m face (never mix silently).
- Nominal annual semiannual YTM for simple benchmark.
- Clean versus dirty bond price and accrued interest.
- Settlement/trade dates, coupon dates, day count and business days.
- Price source: actual bond prices vs. synthetic curve-based valuation.
- Discount-curve compounding and interpolation, and how dates map to t.
- 1bp shock convention; parallel YTM vs. full zero-curve shift.

## I. Numerical engineering rules

Use `double` for calculations and `std::vector` for cash flows. Validate inputs with explicit errors. Avoid using raw owning pointers for simple numerical collections. Use `std::size_t` for container indices and `std::pow` with clear exponents. Compare floating-point outputs with tolerances rather than exact `==`.
