#pragma once

#include "TypeConversion.h"
#include <mono/metadata/class.h>
#include <string>

namespace Mono {
    template<typename Signature, Signature &signature>
    struct InternalCallWrapper;

    template<typename... Args, void (&func)(Args...)>
    struct InternalCallWrapper<void(Args...), func> {
        static void wrapper(typename ConvertType<std::decay_t<Args>>::MonoUnboxedType... args) {
            return func(ConvertType<std::decay_t<Args>>::fromMonoUnboxed(std::move(args))...);
        }
    };

    template<typename RetType, typename... Args, RetType (&func)(Args...)>
    struct InternalCallWrapper<RetType(Args...), func> {
        static typename ConvertType<std::decay_t<RetType>>::MonoUnboxedType
        wrapper(typename ConvertType<std::decay_t<Args>>::MonoUnboxedType... args) {
            return ConvertType<std::decay_t<RetType>>::toMono(
                    func(ConvertType<std::decay_t<Args>>::fromMonoUnboxed(std::move(args))...));
        }
    };

    template<typename Func>
    inline void registerInternalMethodCall(const std::string &name, Func &&func) {
        mono_add_internal_call(name.c_str(), reinterpret_cast<const void *>(func));
    }
}

#define internalCall(func) (&Mono::InternalCallWrapper<decltype(func), func>::wrapper)