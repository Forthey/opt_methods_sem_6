#pragma once

#include "SimpleFunctionWrapper.h"

template<std::uint8_t maxDerivative = 0, std::uint8_t argNum = 2>
class FunctionWithDerWrapper {
    std::array<SimpleFunctionWrapper<argNum>, maxDerivative + 1> function;

public:
    class FunctionWrapperException : public std::exception {
        std::string const msg;

    public:
        explicit FunctionWrapperException(std::string msg) : msg(std::move(msg)) {
        }

        [[nodiscard]] char const *what() const noexcept override { return msg.c_str(); }
    };

    explicit FunctionWithDerWrapper(std::vector<SimpleFunctionWrapper<argNum> > functions)
        : function(std::move(functions)) {
    }

    template<
        typename... FunctionWithDerivatives,
        typename = std::enable_if_t<(std::is_same_v<FunctionWithDerivatives, SimpleFunctionWrapper<argNum> > && ...)> >
    explicit FunctionWithDerWrapper(FunctionWithDerivatives... function) : function{function...} {
        static_assert(sizeof...(FunctionWithDerivatives) == maxDerivative + 1, "Incorrect number of arguments!");
    }

    SimpleFunctionWrapper<argNum> const &operator[](std::uint8_t derivativeOrder = 0) const {
        if (derivativeOrder >= function.size()) {
            throw FunctionWrapperException(
                std::format(
                    "Function derivative order out of range, max {} is present, {} requested",
                    function.size() - 1, derivativeOrder
                )
            );
        }

        return function[derivativeOrder];
    }
};
