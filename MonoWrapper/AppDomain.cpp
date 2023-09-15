#include "AppDomain.h"
#include "Assembly.h"
#include "Thread.h"
#include "Object.h"
#include "Exceptions/FileLoadException.h"
#include "Exceptions/FileNotFoundException.h"
#include <mono/metadata/appdomain.h>
#include <stdexcept>
#include <filesystem>

using namespace Mono;

AppDomain::AppDomain(non_owning_ptr<MonoDomain> domain) : _domain(domain) {
    if (domain == nullptr) {
        throw std::invalid_argument("domain cannot be null");
    }
}

non_owning_ptr<MonoDomain> AppDomain::get() const {
    return _domain;
}

const AppDomain &AppDomain::getCurrentDomain() {
    return Thread::getDomain();
}

AppDomain AppDomain::createDomain(const std::string &friendlyName) {
    non_owning_ptr<MonoDomain> domain = mono_domain_create_appdomain(const_cast<char *>(friendlyName.c_str()), nullptr);
    if (domain == nullptr) {
        // TODO: Throw exception
    }

    mono_bool ret = mono_domain_set(domain, 0);
    if (ret) {
        mono_thread_attach(domain);
    }
    return AppDomain(domain);
}

std::vector<Assembly> AppDomain::getAssemblies() const {
    std::vector<Assembly> assemblies;
    assemblies.reserve(_assemblies.size());
    for (const auto &pair: _assemblies) {
        assemblies.push_back(pair.second);
    }
    return assemblies;
}

Assembly AppDomain::loadAssembly(const std::string &path) {
    // Check assembly already loaded
    auto it = _assemblies.find(path);
    if (it != _assemblies.end()) {
        return it->second;
    }

    // Check assembly file exists
    if (!std::filesystem::exists(path)) {
        throw FileNotFoundException("Assembly file not found: " + path);
    }

    non_owning_ptr<MonoAssembly> assemblyPtr = mono_domain_assembly_open(_domain, path.c_str());
    if (assemblyPtr == nullptr) {
        throw FileLoadException("Failed to load assembly: " + path);
    }

    Assembly assembly(assemblyPtr);
    _assemblies.emplace(path, assembly);
    return assembly;
}

void AppDomain::setConfig(const std::string &configPath) {
    if (!std::filesystem::exists(configPath)) {
        throw FileNotFoundException("Config file not found: " + configPath);
    }

    std::string dirPath = configPath.substr(0, configPath.find_last_of("/\\"));
    std::string fileName = configPath.substr(configPath.find_last_of("/\\") + 1);
    mono_domain_set_config(_domain, dirPath.c_str(), fileName.c_str());
}