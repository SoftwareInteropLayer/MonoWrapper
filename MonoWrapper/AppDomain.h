#pragma once

#include "Assembly.h"
#include "Object.h"
#include "NonOwningPtr.h"
#include "MethodInfo.h"
#include "MethodInvoker.h"
#include "Exceptions/TargetInvocationException.h"
#include <mono/metadata/appdomain.h>
#include <string>
#include <unordered_map>

namespace Mono {
    class Type;
    class Object;

    class AppDomain {
    public:
        explicit AppDomain(non_owning_ptr<MonoDomain> domain);

        /**
         * @brief Get raw MonoDomain pointer.
         * @return Raw MonoDomain pointer.
         */
        non_owning_ptr<MonoDomain> get() const;

        /**
         * @brief Get current domain.
         * @return Current domain.
         */
        static const AppDomain& getCurrentDomain();

        /**
         * @brief Create new domain.
         * @param friendlyName Friendly name of domain.
         * @return New domain created.
         */
        static AppDomain createDomain(const std::string &friendlyName);

        /**
         * @brief Get all assemblies loaded in this domain.
         * @return List of assemblies loaded in this domain.
         */
        std::vector<Assembly> getAssemblies() const;

        /**
         * @brief Load specified assembly file into this domain.
         * @param path Path to assembly file.
         * @return Assembly loaded.
         * @note If assembly is already loaded, it will return the loaded assembly.
         * @throws FileNotFoundException If assembly file not found.
         * @throws FileLoadException If assembly cannot be loaded. (e.g. invalid assembly file)
         */
        Assembly loadAssembly(const std::string &path);

        /**
         * @brief Create new instance of specified type.
         * @tparam Args Types of arguments.
         * @param type Type to create instance.
         * @param args Arguments to pass to constructor.
         * @return Instance created.
         * @throws MissingMethodException If constructor not found.
         * @throws TargetInvocationException If exception occurred when creating instance.
         * @throws ThunkException If exception occurred in constructor.
         */
        template<typename... Args>
        Object createInstance(Type type, Args... args) {
            non_owning_ptr<MonoObject> objectPtr = mono_object_new(_domain, type.get());
            if (objectPtr == nullptr) {
                throw TargetInvocationException("Failed to create instance");
            }

            Object object(type, objectPtr);
            MethodInfo constructor = type.getConstructor(sizeof...(args));
            MethodInvoker<void(Args...)> invoker = makeMethodInvoker<void(Args...)>(constructor);
            invoker(object, std::forward<Args>(args)...);
            return object;
        }

    private:
        non_owning_ptr<MonoDomain> _domain = nullptr;
        mutable std::unordered_map<std::string, Assembly> _assemblies;
    };
}