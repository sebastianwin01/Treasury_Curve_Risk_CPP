
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
          couponsPerYear_(couponsPerYear) {
        // 1: Verify that the face value is finite.
        bool fineteFace = std::isfinite(faceValueUsd_);

        // 2: Verify that the face value is greater than zero.
        bool positiveFace =  faceValueUsd_ > 0;

        // 3: Verify that the annual coupon rate is finite.
        bool fineteAnnual = std::isfinite(annualCouponRate_);

        // 4: Verify that the annual coupon rate is not negative.
        bool positiveCoupon = annualCouponRate_ >= 0;

        //  5: Verify that maturity is finite.
        bool finiteMaturity = std::isfinite(maturityYears_);

        // 6: Verify that maturity is greater than zero.
        bool positiveMaturity = maturityYears_ >0;

        // 7: Verify that couponsPerYear is positive.
        bool positiveCouponsPerYear = couponsPerYear_ > 0;

        // 8: Calculate the total number of coupon periods.
        //         Multiply maturityYears by couponsPerYear.
        double totalPeriods = maturityYears_ * couponsPerYear_;

        //  9: Verify that the number of periods is
        //         a positive whole number.
        //         Account for floating-point precision.

        bool validPeriods =
        std::isfinite(totalPeriods) &&
        totalPeriods >= 1.0 &&
        std::abs(totalPeriods - std::round(totalPeriods)) <= 1e-9;

        //10: Throw std::invalid_argument when
        //an input violates these requirements.
        if (!(fineteFace && positiveFace && fineteAnnual &&
              positiveCoupon && finiteMaturity &&
              positiveMaturity && positiveCouponsPerYear &&
              validPeriods))
        {

            throw std::invalid_argument("Invalid bond parameters");
        }
    }


    // =========================================
    // 2. Getters
    // =========================================

    double Bond::faceValueUsd() const noexcept
    {
        //Return the bond's face value.
        return faceValueUsd_;
    }

    double Bond::annualCouponRate() const noexcept
    {
        // Return the annual coupon rate.
        return annualCouponRate_;
    }

    double Bond::maturityYears() const noexcept
    {
        // : Return the years until maturity.
        return maturityYears_;
    }

    int Bond::couponsPerYear() const noexcept
    {
        //  Return the number of coupons paid per year.
        return couponsPerYear_;
    }

    int Bond::totalPeriods() const noexcept {
        return static_cast<int>(
            std::round(maturityYears_ * couponsPerYear_)
        );
    }


    // =========================================
    // 3. Cash Flow Generation
    // =========================================

    std::vector<CashFlow> Bond::cashFlows() const
    {
        // 1. Get the number of payment periods.
        int periods = totalPeriods();

        // 2. Calculate the coupon payment per period.
        double couponPayment =
            (faceValueUsd_ * annualCouponRate_) / couponsPerYear_;

        // 3. Create an empty vector.
        std::vector<CashFlow> flows;

        // 4. Reserve memory for all payments.
        flows.reserve(periods);

        // 5. Generate each payment.
        for (int i = 1; i <= periods; ++i) {

            // Calculate payment time.
            double time = static_cast<double>(i) / couponsPerYear_;

            // Calculate payment amount.
            double amount = couponPayment;

            // Add principal to final payment.
            if (i == periods) {
                amount += faceValueUsd_;
            }

            // Store the payment.
            flows.push_back({time, amount});
        }

        // 6. Return all cash flows.
        return flows;

    }
}