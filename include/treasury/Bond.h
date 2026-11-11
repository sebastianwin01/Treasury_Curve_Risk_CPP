#pragma once

#include <vector>

namespace treasury {

struct CashFlow {
    double timeYears{};
    double amountUsd{};
};

template <typename T>
class Bond {
private:
    T faceValueUsd_;
    T annualCouponRate_;
    T maturityYears_;
    int couponsPerYear_;

public:
    Bond(T faceValueUsd,
         T annualCouponRate,
         T maturityYears,
         int couponsPerYear);

    T faceValueUsd() const noexcept;
    T annualCouponRate() const noexcept;
    T maturityYears() const noexcept;
    int couponsPerYear() const noexcept;
};

}