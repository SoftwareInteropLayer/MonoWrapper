#pragma once

#include "Object.h"
#include "Thread.h"
#include "AppDomain.h"

namespace Mono {
    class Type;
    class Object;

    class Activator {
    public:
        /**
         * @brief Create new instance of specified type.
         * @tparam Args Types of arguments.
         * @param type Type to create instance.
         * @param args Arguments to pass to constructor.
         * @return Instance created.
         * @note The instance is created in current domain.
         * @throws MissingMethodException If constructor not found.
         * @throws TargetInvocationException If exception occurred when creating instance.
         * @throws ThunkException If exception occurred in constructor.
         */
        template<typename... Args>
        static Object createInstance(const Type &type, Args... args) {
            AppDomain domain = Thread::getDomain();
            return domain.createInstance(type, std::forward<Args>(args)...);
        }
    };
}