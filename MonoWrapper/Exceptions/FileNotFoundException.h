#pragma once

#include "MonoException.h"

namespace Mono {
    class FileNotFoundException : public MonoException {
    public:
        explicit FileNotFoundException(std::string message) : MonoException(std::move(message)) {}
    };
}