#pragma once

#include "MonoException.h"
#include "../MethodInfo.h"

namespace Mono {
    class ThunkException : public MonoException {
    public:
        explicit ThunkException(const MethodInfo &method) : MonoException("Thunk failed"), method(method) {}

        const MethodInfo &getMethod() const {
            return method;
        }

    protected:
        const MethodInfo &method;
    };
}