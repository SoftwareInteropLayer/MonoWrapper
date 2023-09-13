#include "MemberInfo.h"
#include "Type.h"

using namespace Mono;

MemberInfo::MemberInfo(const Type& declaringType, const std::string &name) : _declaringType(declaringType), _name(std::move(name)) {
}

std::string MemberInfo::getName() const {
    return _name;
}

const Type &MemberInfo::getDeclaringType() const {
    return _declaringType;
}

bool MemberInfo::operator==(const MemberInfo &other) const {
    return _declaringType == other._declaringType && _name == other._name;
}

bool MemberInfo::operator!=(const MemberInfo &other) const {
    return !(*this == other);
}