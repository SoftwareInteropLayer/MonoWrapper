#pragma once

#include "Type.h"
#include "NonOwningPtr.h"
#include <mono/metadata/object.h>

namespace Mono {
    class Object {
    public:
        explicit Object(const Type& type, non_owning_ptr<MonoObject> object);

        /**
         * @brief Get raw MonoObject pointer.
         * @return Raw MonoObject pointer.
         */
        non_owning_ptr<MonoObject> get() const;

        /**
         * @brief Get type of this object.
         * @return Type of this object.
         */
        const Type& getType() const;

    protected:
        const Type& _type;
        non_owning_ptr<MonoObject> _object = nullptr;
    };
}