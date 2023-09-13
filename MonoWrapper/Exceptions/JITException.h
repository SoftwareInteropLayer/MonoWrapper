#pragma once

#include "MonoException.h"

namespace Mono {
    class JITException : public MonoException {
    public:
        explicit JITException(std::string message) : MonoException(std::move(message)) {}
    };
}