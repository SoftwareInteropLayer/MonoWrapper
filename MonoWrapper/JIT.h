#pragma once

#include "AppDomain.h"
#include <string>

namespace Mono {
    class AppDomain;

    class JIT {
    public:
        /**
         * @brief Initialize the Mono JIT.
         * @param domainName The name of the JIT domain.
         * @param monoInstallationPath The path to the mono installation directory. (e.g. C:\Program Files\Mono)
         * @return The created JIT domain.
         * @throws JITException If the JIT domain could not be created.
         * @throws std::runtime_error If the JIT has already been initialized.
         */
        static AppDomain initialize(const std::string &domainName, const std::string &monoInstallationPath);

        /**
         * @brief Cleanup the Mono JIT.
         * @note This function should be called before the program exits.
         */
        static void cleanup();
    };
}