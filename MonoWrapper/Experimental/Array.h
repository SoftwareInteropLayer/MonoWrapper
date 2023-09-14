#pragma once

#include "../Type.h"
#include "../NonOwningPtr.h"
#include <mono/metadata/appdomain.h>
#include <stdexcept>

namespace Mono::Experimental {
    template<typename T>
    struct ConvertType;

    class Array {
    public:
        explicit Array(non_owning_ptr<MonoArray> array);

        /**
         * @brief Create a new array with specified type and size.
         * @param type Type of array.
         * @param size Size of array.
         * @return New array.
         * @throws TargetInvocationException If failed to create array.
         */
        static Array newArray(const Type &type, int size);

        /**
         * @brief Create a new array with specified size. (Type is System.Object)
         * @param size Size of array.
         * @return New array.
         * @throws TargetInvocationException If failed to create array.
         */
        static Array newArray(int size);

        /**
         * @brief Get raw MonoArray pointer.
         * @return Raw MonoArray pointer.
         */
        non_owning_ptr<MonoArray> get() const;

        /**
         * @brief Get count of elements in array.
         * @return Count of elements in array.
         */
        int count() const;

        template<typename T>
        void set(int index, const T &value) const {
            if (index < 0 || index >= _count) {
                throw std::out_of_range("index out of range");
            }
            auto element = ConvertType<std::decay_t<T>>::toMono(value);
            mono_array_setref(_array, index, element);
        }

    protected:
        non_owning_ptr<MonoArray> _array = nullptr;
        int _count = 0;
    };
}