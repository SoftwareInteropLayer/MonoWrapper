#include "StringHelper.h"
#include <algorithm>
#include <cctype>

using namespace Mono;

std::string StringHelper::toLower(const std::string &str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}