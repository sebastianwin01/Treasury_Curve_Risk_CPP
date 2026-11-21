//
// Created by sebastian on 11/20/26.
//

#ifndef TREASURYCURVERISK_CURVETRADE_H
#define TREASURYCURVERISK_CURVETRADE_H

#pragma once
#include "treasury/Bond.h"

namespace treasury {

    struct TradeLeg {
        Bond bond;
        double startingYtmDecimal;
        double signedFaceNotionalUsd; // >0 long, <0 short
    };

    struct CurveTrade {
        TradeLeg twoYear;
        TradeLeg tenYear;
    };

    CurveTrade makeDv01Steepener(
        const Bond& bond2y, double ytm2y,
        const Bond& bond10y, double ytm10y,
        double twoYearAnchorFaceUsd);

    CurveTrade makeDv01Flattener(
        const Bond& bond2y, double ytm2y,
        const Bond& bond10y, double ytm10y,
        double twoYearAnchorFaceUsd);

    double signedParallelDv01UsdPerBp(const CurveTrade& trade);

} // namespace treasury

#endif //TREASURYCURVERISK_CURVETRADE_H