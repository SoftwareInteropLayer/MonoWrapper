#pragma once

#include "MonoException.h"

namespace Mono {
    class FileLoadException : public MonoException {
    public:
        explicit FileLoadException(std::string message) : MonoException(std::move(message)) {}
    };
}