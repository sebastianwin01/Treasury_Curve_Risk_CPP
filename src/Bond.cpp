
#include "treasury/Bond.h"
#include <stdexcept>
#include <cmath>

namespace treasury {

// =========================================
// 1. Constructor
// =========================================

Bond::Bond(double faceValueUsd,
           double annualCouponRate,
           double maturityYears,
           int couponsPerYear)
    : faceValueUsd_(faceValueUsd),
      annualCouponRate_(annualCouponRate),
      maturityYears_(maturityYears),
      couponsPerYear_(couponsPerYear)
{
    // TODO 1: Verify that the face value is finite.
    // TODO 2: Verify that the face value is greater than zero.

    // TODO 3: Verify that the annual coupon rate is finite.
    // TODO 4: Verify that the annual coupon rate is not negative.
    //         A zero coupon rate must be allowed.

    // TODO 5: Verify that maturity is finite.
    // TODO 6: Verify that maturity is greater than zero.

    // TODO 7: Verify that couponsPerYear is positive.

    // TODO 8: Calculate the total number of coupon periods.
    //         Multiply maturityYears by couponsPerYear.

    // TODO 9: Verify that the number of periods is
    //         a positive whole number.
    //         Account for floating-point precision.

    // TODO 10: Throw std::invalid_argument when
    //          an input violates these requirements.
}


// =========================================
// 2. Getters
// =========================================

double Bond::faceValueUsd() const noexcept
{
    // TODO: Return the bond's face value.
}

double Bond::annualCouponRate() const noexcept
{
    // TODO: Return the annual coupon rate.
}

double Bond::maturityYears() const noexcept
{
    // TODO: Return the years until maturity.
}

int Bond::couponsPerYear() const noexcept
{
    // TODO: Return the number of coupons paid per year.
}


// =========================================
// 3. Cash Flow Generation
// =========================================

std::vector<CashFlow> Bond::cashFlows() const
{
    // TODO 1: Determine the total number
    //         of coupon payment periods.

    // TODO 2: Calculate the coupon amount
    //         paid during each period.
    //
    //         Consider:
    //         - Face value
    //         - Annual coupon rate
    //         - Number of payments per year

    // TODO 3: Create an empty vector
    //         that will store CashFlow objects.

    // TODO 4: Iterate through every payment period,
    //         starting with the first payment.

    // TODO 5: For each period, calculate the
    //         time in years when payment occurs.

    // TODO 6: Determine the payment amount
    //         for that period.

    // TODO 7: If this is the final period,
    //         add the bond's face value
    //         to the final coupon payment.

    // TODO 8: Create a CashFlow object containing
    //         the payment time and amount.

    // TODO 9: Add the CashFlow object to the vector.

    // TODO 10: Return the completed vector.
}
