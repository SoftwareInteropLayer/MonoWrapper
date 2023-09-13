#pragma once

#include "MonoException.h"

namespace Mono {
    class AccessViolationException : public MonoException {
    public:
        explicit AccessViolationException(std::string message) : MonoException(std::move(message)) {}
    };
}