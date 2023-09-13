#pragma once

#include <string>

namespace Mono {
    class StringHelper {
    public:
        /**
         * @brief Transform a string to lower case
         * @param str The string to transform
         * @return The transformed string
         */
        static std::string toLower(const std::string &str);
    };
}