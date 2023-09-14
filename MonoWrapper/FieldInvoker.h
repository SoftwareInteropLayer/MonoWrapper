#pragma once

#include "TypeConversion.h"
#include <mono/metadata/object.h>
#include <memory>

namespace Mono {
    class Object;

    class FieldInfo;

    template<typename T>
    class FieldInvoker {
    public:
        explicit FieldInvoker(non_owning_ptr<MonoClassField> field) : _field(field) {}

        void setValue(const Object &object, T value) const {
            setValueImpl(&object, std::forward<T>(value));
        }

        T getValue(const Object &object) const {
            return getValueImpl(&object);
        }

    private:
        void setValueImpl(const Object *object, T value) const {
            auto monoValue = ConvertType<std::decay_t<T>>::toMono(std::forward<T>(value));
            auto arg = toMonoArg(monoValue);

            MonoObject *obj = object->get();
            mono_field_set_value(obj, _field, arg);
        }

        T getValueImpl(const Object *object) const {
            throw std::runtime_error("Not implemented");
        }

    protected:
        non_owning_ptr<MonoClassField> _field;
    };

    /**
     * @brief Create a FieldInvoker Helper. This helper can be used to get/set value of a field.
     * @param field The mono field to get/set value.
     * @return The FieldInvoker Helper.
     */
    template<typename T>
    FieldInvoker<T> makeFieldInvoker(non_owning_ptr<MonoClassField> field) {
        return FieldInvoker<T>(field);
    }
}