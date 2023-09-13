#include "JIT.h"
#include "AppDomain.h"
#include "Thread.h"
#include "StringHelper.h"
#include "Exceptions/JITException.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/utils/mono-logger.h>
#include <stdexcept>
#include <algorithm>

using namespace Mono;

static non_owning_ptr<MonoDomain> jitDomainPtr = nullptr;

AppDomain JIT::initialize(const std::string &domainName, const std::string &monoInstallationPath) {
    if (jitDomainPtr != nullptr) {
        throw std::runtime_error("JIT has already been initialized");
    }

    std::string monoPathFormatted = monoInstallationPath;
    // Remove trailing slash if present
    if (monoPathFormatted.back() == '/') {
        monoPathFormatted.pop_back();
    }

    std::string monoLibPath = monoPathFormatted + "/lib";
    std::string monoEtcPath = monoPathFormatted + "/etc";
    mono_set_dirs(monoLibPath.c_str(), monoEtcPath.c_str());

    mono_config_parse(nullptr);

    jitDomainPtr = mono_jit_init(domainName.c_str());
    if (jitDomainPtr == nullptr) {
        throw JITException("Failed to initialize JIT");
    }

    mono_thread_set_main(mono_thread_current());

    AppDomain jitDomain(jitDomainPtr);
    Thread::setDomain(jitDomain);
    return jitDomain;
}

void JIT::cleanup() {
    if (jitDomainPtr != nullptr) {
        mono_jit_cleanup(jitDomainPtr);
    }
    jitDomainPtr = nullptr;
}