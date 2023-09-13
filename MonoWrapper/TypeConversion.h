#pragma once

#include "Object.h"
#include "Experimental/Array.h"
#include <mono/jit/jit.h>
#include <memory>
#include <iostream>

namespace Mono {
    template<typename T>
    inline auto toMonoArg(T &value) {
        return std::addressof(value);
    }

    inline auto toMonoArg(MonoObject *value) {
        return value;
    }

    template<typename T>
    struct ConvertType {
        using CppType = T;
        using MonoUnboxedType = T;
        using MonoBoxedType = MonoObject *;

        static MonoUnboxedType toMono(const CppType &value) {
            return value;
        }

        static CppType fromMonoUnboxed(const MonoUnboxedType &value) {
            return value;
        }

        static CppType fromMonoBoxed(const MonoBoxedType &value) {
            void *ptr = mono_object_unbox(value);
            return *reinterpret_cast<CppType *>(ptr);
        }
    };

    template<>
    struct ConvertType<Object> {
        using CppType = Object;
        using MonoUnboxedType = MonoObject *;
        using MonoBoxedType = MonoObject *;

        static MonoUnboxedType toMono(const CppType &value) {
            return value.get();
        }

        static CppType fromMonoUnboxed(const MonoUnboxedType &value) {
            auto typePtr = mono_object_get_class(value);
            auto type = Type(typePtr);
            return CppType(type, value);
        }

        static CppType fromMonoBoxed(const MonoBoxedType &value) {
            auto typePtr = mono_object_get_class(value);
            auto type = Type(typePtr);
            return CppType(type, value);
        }
    };

    template<>
    struct ConvertType<Experimental::Array> {
        using CppType = Experimental::Array;
        using MonoUnboxedType = MonoObject *;
        using MonoBoxedType = MonoObject *;

        static MonoUnboxedType toMono(const CppType &value) {
            return reinterpret_cast<MonoObject *>(value.get());
        }

        static CppType fromMonoUnboxed(const MonoUnboxedType &value) {
            auto array = reinterpret_cast<MonoArray *>(value);
            return Experimental::Array(array);
        }

        static CppType fromMonoBoxed(const MonoBoxedType &value) {
            auto array = reinterpret_cast<MonoArray *>(value);
            return Experimental::Array(array);
        }
    };

    template<>
    struct ConvertType<std::string> {
        using CppType = std::string;
        using MonoUnboxedType = MonoObject *;
        using MonoBoxedType = MonoObject *;

        static MonoUnboxedType toMono(const CppType &value) {
            auto str = mono_string_new(mono_domain_get(), value.c_str());
            return reinterpret_cast<MonoObject *>(str);
        }

        static CppType fromMonoUnboxed(const MonoUnboxedType &value) {
            auto monoStr = reinterpret_cast<MonoString *>(value);
            auto rawStr = mono_string_to_utf8(monoStr);
            std::string str = rawStr;
            mono_free(rawStr);
            return str;
        }

        static CppType fromMonoBoxed(const MonoBoxedType &value) {
            auto monoStr = reinterpret_cast<MonoString *>(value);
            auto rawStr = mono_string_to_utf8(monoStr);
            std::string str = rawStr;
            mono_free(rawStr);
            return str;
        }
    };

    template<>
    struct ConvertType<char *> {
        using CppType = std::string;
        using MonoUnboxedType = MonoObject *;
        using MonoBoxedType = MonoObject *;

        static MonoUnboxedType toMono(const CppType &value) {
            auto str = mono_string_new(mono_domain_get(), value.c_str());
            return reinterpret_cast<MonoObject *>(str);
        }

        static CppType fromMonoUnboxed(const MonoUnboxedType &value) {
            auto monoStr = reinterpret_cast<MonoString *>(value);
            auto rawStr = mono_string_to_utf8(monoStr);
            std::string str = rawStr;
            mono_free(rawStr);
            return str;
        }

        static CppType fromMonoBoxed(const MonoBoxedType &value) {
            auto monoStr = reinterpret_cast<MonoString *>(value);
            auto rawStr = mono_string_to_utf8(monoStr);
            std::string str = rawStr;
            mono_free(rawStr);
            return str;
        }
    };
}