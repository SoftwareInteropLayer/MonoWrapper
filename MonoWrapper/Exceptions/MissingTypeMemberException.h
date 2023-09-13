#pragma once

#include "MonoException.h"

namespace Mono {
    class MissingTypeMemberException : public MonoException {
    public:
        explicit MissingTypeMemberException(std::string message) : MonoException(std::move(message)) {}
    };
}