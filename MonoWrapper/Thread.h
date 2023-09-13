#pragma once

#include "NonOwningPtr.h"
#include <mono/metadata/threads.h>

namespace Mono {
    class AppDomain;

    class Thread {
    public:
        explicit Thread(non_owning_ptr<MonoThread> thread);

        static const AppDomain& getDomain();

        static void setDomain(const AppDomain& domain);

        static Thread currentThread();

        static Thread attach(const AppDomain& domain);

        void detach();

    protected:
        non_owning_ptr<MonoThread> _thread = nullptr;
    };
}