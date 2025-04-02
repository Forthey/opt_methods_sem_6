#pragma once
#include <functional>
#include <format>

#include "RealVector.h"


template<std::uint8_t argNum = 2, typename ReturnValue = double>
class FunctionWrapper {
    using Function = std::function<ReturnValue(RealVector<argNum> const &)>;
    using ValidatorFunction = std::function<bool(RealVector<argNum> const &)>;

    Function function;
    ValidatorFunction validator;
    std::uint64_t useCount = 0ull;

    static bool standardValidator(RealVector<argNum> const &x) {
        for (std::uint8_t i = 0; i < argNum; ++i) {
            if (!std::isfinite(x[i])) {
                return false;
            }
        }
        return true;
    }

public:
    class FunctionValidationException : public std::exception {
        std::string const msg;

    public:
        explicit FunctionValidationException(RealVector<argNum> const &x)
            : msg(std::format("Validation error for x = {}", x.toString())) {
        }

        [[nodiscard]] char const *what() const noexcept override { return msg.c_str(); }
    };

    explicit FunctionWrapper(Function function, ValidatorFunction validator = standardValidator)
        : function(std::move(function)), validator(std::move(validator)) {
    }

    ReturnValue operator()(RealVector<argNum> const &x) {
        if (!validator(x)) {
            throw FunctionValidationException(x);
        }
        ++useCount;

        return function(x);
    }

    template<
        typename... Values,
        typename = std::enable_if_t<(std::is_same_v<Values, double> && ...)> >
    ReturnValue operator()(Values... x) {
        static_assert(sizeof...(Values) == argNum, "Incorrect number of arguments!");

        return operator()(RealVector<argNum>{x...});
    }

    [[nodiscard]] std::uint64_t getUseCount() const { return useCount; }

    void resetUseCount() { useCount = 0ull; }
};
