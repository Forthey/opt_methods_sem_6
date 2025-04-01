#include <cstdlib>
#include <cmath>
#include <iostream>

#include "alg/FunctionWrapper.h"
#include "function.h"


int main() {
    FunctionWithDerWrapper<2> f(
        SimpleFunctionWrapper<>(function, standardValidator),
        SimpleFunctionWrapper<>(functionDerivative, standardValidator),
        SimpleFunctionWrapper<>(functionDerivative2, standardValidator)
    );

    std::cout << f[1](1.0, 1.0) << std::endl;

    return EXIT_SUCCESS;
}
