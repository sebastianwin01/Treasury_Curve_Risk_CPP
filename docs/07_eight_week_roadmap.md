# 07 — Concrete eight-week roadmap (40–56 planned hours)

**Budget:** roughly 5–7 hours/week **including finance study, coding and testing**. Eight phases correspond to eight small deliverables rather than an ambition to build an institutional bond engine.

| Week / phase | Deliverable | Exit criterion |
|---|---|---|
| **1 — Coupon cash flows and PV** | `Bond`, `BondPricing`, first tests | $98.1190129 discount example and $100 par case |
| **2 — YTM solver** | `solveYTMFromPrice()` bisection | Recover 5% from known price; invalid bracket reported |
| **3 — Risk measures** | `BondRisk` duration/convexity/DV01 | Finite differences cross-check derivatives and units |
| **4 — Trade construction** | `CurveTrade` long/short 2Y–10Y | Match **signed parallel DV01** at inception |
| **5 — Shock analysis** | `ScenarioAnalysis` | Parallel/steepening/flattening table with first-order and exact repricing |
| **6 — Harden and test** | CTest regression suite; error paths | Clean build, no silent invalid-input behavior |
| **7 — Show results** | Reproducible table + plot/report draft; **optional** descriptive spread | Clearly identified synthetic scenarios and assumptions |
| **8 — Final delivery** | README, technical discussion, limitations and FQE slides | A reviewer can reproduce the output and calculations |

## Finance reading to implementation mapping

- Week 1: Tuckman & Serrat Chapter 1 — coupon payments, PV, discounting.
- Week 2: Chapter 3 — YTM.
- Week 3: Chapter 4 — duration, convexity, DV01.
- Week 4–5: Chapter 4 and, selectively, Chapter 5 — hedge sizing and exposure.

## MVP acceptance gate after Week 5

1. Bond cash flows, PV, YTM and risk functions validated.
2. Long 2Y / short 10Y and reverse trade positions sized to cancel first-order parallel DV01.
3. **Three scenarios** with positive/negative signs verified.
4. Approximate P&L compared to **full two-YTM repricing**, not mislabeled as real trading results.

## How to protect the time budget

- Do **not** start a coupon-date zero-curve interpolation module until all MVP gates pass.
- Do **not** write a historical backtester, financing ledger, trading signals, or regime model.
- Do **not** depend on fetching market data to demonstrate the project.
- If ahead in Week 7, choose **one** extension: a historical **descriptive** spread plot **or** a tested discount-curve module; do not add both automatically.
- Reserve Week 8 for final reproducibility and communication.

## Weekly work rhythm

~1.5–2 h finance derivation, ~2.5–3 h C++ implementation and ~1–1.5 h testing/documentation. Adjust as needed without compromising basic validation.

See [PROJECT_STATUS.md](../PROJECT_STATUS.md), [technical architecture](11_technical_architecture.md), and [Phase 1 guide](12_start_here.md).
