#pragma once

#include "MethodInfo.h"
#include "TypeConversion.h"
#include "Exceptions/ThunkException.h"
#include <memory>
#include <tuple>
#include <vector>

namespace Mono {
    class Object;

    class MethodInfo;

    template<typename T>
    class MethodInvoker;

    template<typename... Args>
    class MethodInvoker<void(Args...)> {
    public:
        void operator()(Args... args) {
            invoke(nullptr, std::forward<Args>(args)...);
        }

        void operator()(const Object &object, Args... args) {
            invoke(&object, std::forward<Args>(args)...);
        }

    private:
        explicit MethodInvoker(const MethodInfo &method) : _method(method) {}

        void invoke(const Object *object, Args... args) {
            MonoMethod *method = _method.get();
            MonoObject *obj = nullptr;
            if (object) {
                obj = object->get();
                method = mono_object_get_virtual_method(obj, method);
            }

            auto tuple = std::make_tuple(ConvertType<std::decay_t<Args>>::toMono(std::forward<Args>(args))...);
            auto inv = [this, method, obj](auto... args) {
                std::vector<void *> argv = {toMonoArg(args)...};

                non_owning_ptr<MonoObject> exception = nullptr;
                mono_runtime_invoke(method, obj, argv.data(), &exception);
                if (exception) {
                    throw ThunkException(_method);
                }
            };
            std::apply(inv, tuple);
        }

        template<typename Signature>
        friend MethodInvoker<Signature> makeMethodInvoker(const MethodInfo &method);

    protected:
        const MethodInfo &_method;
    };

    template<typename RetType, typename... Args>
    class MethodInvoker<RetType(Args...)> {
    public:
        auto operator()(Args... args) {
            return invoke(nullptr, std::forward<Args>(args)...);
        }

        auto operator()(const Object &object, Args... args) {
            return invoke(&object, std::forward<Args>(args)...);
        }

    private:
        explicit MethodInvoker(const MethodInfo &method) : _method(method) {}

        RetType invoke(const Object *object, Args... args) {
            MonoMethod *method = _method.get();
            MonoObject *obj = nullptr;
            if (object) {
                obj = object->get();
                method = mono_object_get_virtual_method(obj, method);
            }

            auto tuple = std::make_tuple(ConvertType<std::decay_t<Args>>::toMono(std::forward<Args>(args))...);
            auto inv = [this, method, obj](auto... args) {
                std::vector<void *> argv = {toMonoArg(args)...};

                non_owning_ptr<MonoObject> exception = nullptr;
                auto result = mono_runtime_invoke(method, obj, argv.data(), &exception);
                if (exception) {
                    throw ThunkException(_method);
                }
                return result;
            };
            auto result = std::apply(inv, tuple);
            return ConvertType<std::decay_t<RetType>>::fromMonoBoxed(std::move(result));
        }

        template<typename Signature>
        friend MethodInvoker<Signature> makeMethodInvoker(const MethodInfo &method);

    protected:
        const MethodInfo &_method;
    };

    /**
     * @brief Create a MethodInvoker helper. This is a helper function for calling a C# method.
     * @tparam Signature The signature of the method. (e.g. void(int, std::string))
     * @param method The method to call.
     * @return A MethodInvoker helper.
     */
    template<typename Signature>
    MethodInvoker<Signature> makeMethodInvoker(const MethodInfo &method) {
        return MethodInvoker<Signature>(method);
    }
}