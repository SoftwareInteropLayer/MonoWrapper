#include "Array.h"
#include "../Thread.h"
#include "../AppDomain.h"
#include <stdexcept>

using namespace Mono::Experimental;

Array::Array(non_owning_ptr<MonoArray> array) : _array(array) {
    if (array == nullptr) {
        throw std::invalid_argument("array cannot be null");
    }

    _count = (int) mono_array_length(_array);
}

Array Array::newArray(const Type &type, int size) {
    AppDomain domain = Thread::getDomain();
    auto arrayPtr = mono_array_new(domain.get(), type.get(), static_cast<uintptr_t>(size));
    if (arrayPtr == nullptr) {
        throw TargetInvocationException("Failed to create array");
    }
    return Array(arrayPtr);
}

Array Array::newArray(int size) {
    AppDomain domain = Thread::getDomain();
    auto arrayPtr = mono_array_new(domain.get(), mono_get_object_class(), static_cast<uintptr_t>(size));
    if (arrayPtr == nullptr) {
        throw TargetInvocationException("Failed to create array");
    }
    return Array(arrayPtr);
}

non_owning_ptr<MonoArray> Array::get() const {
    return _array;
}

int Array::count() const {
    return _count;
}