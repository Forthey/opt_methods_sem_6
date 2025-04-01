#pragma once
#include <functional>
#include <format>

#include "RealVector.h"


template<std::uint8_t argNum = 2>
class SimpleFunctionWrapper {
    using Function = std::function<double(RealVector<argNum> const &)>;
    using ValidatorFunction = std::function<bool(RealVector<argNum> const &)>;

    Function function;
    ValidatorFunction validator;

public:
    class FunctionValidationException : public std::exception {
        std::string const msg;

    public:
        explicit FunctionValidationException(RealVector<argNum> const &x)
            : msg(std::format("Validation error for x = {}", RealVectorToString<argNum>(x))) {
        }

        [[nodiscard]] char const *what() const noexcept override { return msg.c_str(); }
    };

    SimpleFunctionWrapper(Function function, ValidatorFunction validator)
        : function(std::move(function)), validator(std::move(validator)) {
    }

    double operator()(RealVector<argNum> const &x) const {
        if (!validator(x)) {
            throw FunctionValidationException(x);
        }

        return function(x);
    }

    template<
        typename... Values,
        typename = std::enable_if_t<(std::is_same_v<Values, double> && ...)> >
    double operator()(Values... x) const {
        static_assert(sizeof...(Values) == argNum, "Incorrect number of arguments!");

        return operator()(RealVector<argNum>{x...});
    }
};
