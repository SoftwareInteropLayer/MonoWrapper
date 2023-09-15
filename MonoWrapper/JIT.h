#pragma once

#include "AppDomain.h"
#include <string>

namespace Mono {
    class AppDomain;

    class JIT {
    public:
        using LoggerCallback = void (*)(const char *, const char *, const char *, mono_bool, void *);

        /**
         * @brief Initialize the Mono JIT.
         * @param domainName The name of the JIT domain.
         * @param monoInstallationPath The path to the mono installation directory. (e.g. C:\Program Files\Mono)
         * @param debugLevel The debug level of the mono runtime. (e.g. info, warning, error)
         * @param logCallback The callback function to handle the mono log.
         * @return The created JIT domain.
         * @throws JITException If the JIT domain could not be created.
         * @throws std::runtime_error If the JIT has already been initialized.
         */
        static AppDomain initialize(const std::string &domainName, const std::string &monoInstallationPath,
                                    const std::string &debugLevel = "warning", LoggerCallback logCallback = nullptr);

        /**
         * @brief Cleanup the Mono JIT.
         * @note This function should be called before the program exits.
         */
        static void cleanup();
    };
}