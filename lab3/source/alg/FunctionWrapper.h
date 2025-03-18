#pragma once
#include <string>
#include <functional>
#include <format>


class FunctionWrapper {
    std::size_t useCount = 0ull;

    std::function<double(double)> function;
    std::function<bool(double)> validator;

public:
    class ValidationException : public std::exception {
        std::string const message;

    public:
        explicit ValidationException(double const x)
            : message(std::format("Validation error for x = {}", x)) {
        }

        [[nodiscard]] const char *what() const noexcept override {
            return message.c_str();
        }
    };

    explicit FunctionWrapper(std::function<double(double)> function,
                             std::function<bool(double)> validator)
        : function(std::move(function)), validator(std::move(validator)) {
    }

    [[nodiscard]] double operator()(const double x) {
        ++useCount;
        if (!validator(x)) {
            throw ValidationException(x);
        }
        return function(x);
    }

    [[nodiscard]] std::size_t getUseCount() const noexcept {
        return useCount;
    }

    void resetUseCount() noexcept {
        useCount = 0ull;
    }
};
