#pragma once

#include "MonoException.h"

namespace Mono {
    class MissingAttributeException : public MonoException {
    public:
        explicit MissingAttributeException(std::string message) : MonoException(std::move(message)) {}
    };
}