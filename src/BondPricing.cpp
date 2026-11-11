#include "treasury/BondPricing.h"    // Incluir declaración de priceFromYTM y la clase Bond
#include <cmath>                     // std::pow() - std::isfinite()
#include <stdexcept>                 // Lanzar excepciones para inputs inválidos

namespace treasury {

    double priceFromYTM(const Bond& bond, double annualYtmDecimal) {

        if (!std::isfinite(annualYtmDecimal)) {                                         // Verificar que YTM sea un número válido
            throw std::invalid_argument("YTM must be finite");
        }

        const int frequency = bond.couponsPerYear();                                    // Obtener la frecuencia de pagos por año

        if (frequency <= 0) {                                                           // Verificar que la frecuencia válida
            throw std::invalid_argument("Coupon frequency must be positive");
        }

        const double periodicYtm = annualYtmDecimal / static_cast<double>(frequency);   // Convertir el YTM anual en tasa por período

        const double discountBase = 1.0 + periodicYtm;                                  // Calcular la base (1 + y/m) usada para descontar

        if (discountBase <= 0.0) {                                                      // Verificar base de descuento
            throw std::invalid_argument("Invalid YTM");
        }

        const auto cashFlows = bond.cashFlows();                                        // Obtener todos los pagos futuros generados por Bond

        if (cashFlows.empty()) {                                                        // Verificar que el bono tenga al menos un pago futuro
            throw std::invalid_argument("Bond has no cash flows");
        }

        double price = 0.0;                                                             // Inicializar el valor presente acumulado

        for (const auto& cashFlow : cashFlows) {                                        // Recorrer directamente cada cash flow del bono

            const double periods = cashFlow.timeYears * static_cast<double>(frequency); // Convertir el tiempo del cash flow a número de períodos
            const double discountFactor = std::pow(discountBase, periods);              // Calcular (1 + y/m)^(m*t)

            price += cashFlow.amountUsd / discountFactor;                               // Descontar el cash flow y agregar su valor presente
        }

        return price;                                                                   // Devolver la suma de todos los valores presentes
    }

}