#pragma once

#include "MemberInfo.h"
#include <mono/metadata/object.h>

namespace Mono {
    class FieldInfo : public MemberInfo {
    public:
        explicit FieldInfo(const Type& type, non_owning_ptr<MonoClassField> field);

        /**
         * @brief Get a specific custom attribute with specified type.
         * @param attributeType Type of attribute to get.
         * @return Custom attribute object.
         * @throws MissingAttributeException If attribute not found.
         */
        Object getCustomAttribute(const Type &attributeType) override;

        /**
         * @brief Check if this field has a specific custom attribute with specified type.
         * @param attributeType Type of attribute to check.
         * @return True if this field has a specific custom attribute with specified type, false otherwise.
         */
        bool hasCustomAttribute(const Type &attributeType) override;

    protected:
        non_owning_ptr<MonoClassField> _field;
    };
}