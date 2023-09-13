#pragma once

#include "MonoException.h"

namespace Mono {
    class TargetInvocationException : public MonoException {
    public:
        explicit TargetInvocationException(std::string message) : MonoException(std::move(message)) {}
    };
}