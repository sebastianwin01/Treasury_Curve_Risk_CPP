#pragma once

#include <vector>

namespace treasury {

struct CashFlow {
    double timeYears{};
    double amountUsd{};
};

class Bond {

private:
    double faceValueUsd_;
    double annualCouponRate_;
    double maturityYears_;
    int couponsPerYear_;

public:
    // Constructor
    Bond(double faceValueUsd,
         double annualCouponRate,
         double maturityYears,
         int couponsPerYear);

    // Getters
    double faceValueUsd() const noexcept;
    double annualCouponRate() const noexcept;
    double maturityYears() const noexcept;
    int couponsPerYear() const noexcept;
    int totalPeriods() const noexcept;

    // Cash flow generation
    std::vector<CashFlow> cashFlows() const;
};

}
