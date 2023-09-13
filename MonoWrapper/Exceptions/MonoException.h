#pragma once

#include <exception>
#include <string>
#include <utility>

namespace Mono {
    class MonoException : public std::exception {
    public:
        explicit MonoException(std::string message) : _message(std::move(message)) {}

        const char *what() const noexcept override {
            return _message.c_str();
        }

    protected:
        std::string _message;
    };
}