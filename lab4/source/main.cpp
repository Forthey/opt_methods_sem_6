#include <cstdlib>
#include <cmath>
#include <iostream>

#include "function/FunctionWithDerWrapper.h"
#include "function/function.h"
#include "alg/AlgsForExtremes.h"


int main() {
    FunctionWithDerWrapper<2> func{
            FunctionWrapper<2>(function),
            FunctionWrapper<2>(functionDerivative),
            FunctionWrapper<2>(functionDerivative2)
    };

    std::cout << AlgsForExtremes::gradientMethod<2>(func[0], func[1], 0.1).toString() << std::endl;
    std::cout << AlgsForExtremes::gradientMethod<2>(func[0], func[1], 0.01).toString() << std::endl;
    std::cout << AlgsForExtremes::gradientMethod<2>(func[0], func[1], 0.001).toString() << std::endl;

    return EXIT_SUCCESS;
}
