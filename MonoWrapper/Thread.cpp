#include "Thread.h"
#include "AppDomain.h"
#include <stdexcept>
#include <memory>

using namespace Mono;

static std::unique_ptr<AppDomain> currentDomain;

Thread::Thread(non_owning_ptr<MonoThread> thread) : _thread(thread) {
    if (_thread == nullptr) {
        throw std::invalid_argument("thread cannot be null");
    }
}

const AppDomain &Thread::getDomain() {
    if (currentDomain == nullptr) {
        throw std::runtime_error("Domain not initialized");
    }
    return *currentDomain;
}

void Thread::setDomain(const AppDomain &domain) {
    currentDomain = std::make_unique<AppDomain>(domain);
}

Thread Thread::currentThread() {
    non_owning_ptr<MonoThread> threadPtr = mono_thread_current();
    return Thread(threadPtr);
}

Thread Thread::attach(const AppDomain &domain) {
    non_owning_ptr<MonoThread> threadPtr = mono_thread_attach(domain.get());
    return Thread(threadPtr);
}

non_owning_ptr<MonoThread> Thread::get() const {
    return _thread;
}

void Thread::detach() {
    mono_thread_detach(_thread);
}