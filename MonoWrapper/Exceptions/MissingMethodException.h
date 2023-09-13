#pragma once

#include "MonoException.h"

namespace Mono {
    class MissingMethodException : public MonoException {
    public:
        explicit MissingMethodException(std::string message) : MonoException(std::move(message)) {}
    };
}