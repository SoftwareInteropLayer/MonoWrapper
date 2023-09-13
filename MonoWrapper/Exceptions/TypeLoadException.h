#pragma once

#include "MonoException.h"

namespace Mono {
    class TypeLoadException : public MonoException {
    public:
        explicit TypeLoadException(std::string message) : MonoException(std::move(message)) {}
    };
}