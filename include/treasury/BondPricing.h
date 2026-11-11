#pragma once                    // Evitar que este header sea incluido más de una vez durante la compilación

#include "treasury/Bond.h"      // Incluir la definición de la clase Bond que será usada por la función de pricing

namespace treasury {            // Agrupar las funciones del proyecto dentro del namespace treasury

    double priceFromYTM(        // Declarar una función que calculará y devolverá el precio del bono como double
        const Bond& bond,       // Recibir el bono por referencia constante: se lee sin copiarlo ni modificarlo
        double annualYtmDecimal // Recibir el YTM anual en formato decimal, por ejemplo 0.05 para 5%
    );

}                               // Cerrar el namespace treasury