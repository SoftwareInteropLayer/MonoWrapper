#include "Object.h"
#include <stdexcept>

using namespace Mono;

Object::Object(const Type &type, non_owning_ptr<MonoObject> object) : _type(type), _object(object) {
    if (object == nullptr) {
        throw std::invalid_argument("object is null");
    }
}

non_owning_ptr<MonoObject> Object::get() const {
    return _object;
}

const Type &Object::getType() const {
    return _type;
}