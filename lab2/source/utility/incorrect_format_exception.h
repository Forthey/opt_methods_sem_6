#pragma once
#include <stdexcept>

class IncorrectFormatException : public std::exception {
    std::string const& message;
public:
    IncorrectFormatException(std::string const& filename) : message("Неверный формат файла: " + filename) {}

    char const* what() const noexcept override {
        return message.c_str();
    }
};