#include "Assembly.h"
#include "AppDomain.h"
#include "Exceptions/FileNotFoundException.h"
#include "Exceptions/TypeLoadException.h"
#include <mono/metadata/tokentype.h>
#include <stdexcept>

using namespace Mono;

Assembly::Assembly(non_owning_ptr<MonoAssembly> assembly) : _assembly(assembly) {
    if (_assembly == nullptr) {
        throw std::invalid_argument("assembly cannot be null");
    }
    _image = mono_assembly_get_image(_assembly);
}

non_owning_ptr<MonoAssembly> Assembly::get() const {
    return _assembly;
}

Assembly Assembly::loadFile(const std::string &path) {
    auto domain = AppDomain::getCurrentDomain();
    return domain.loadAssembly(path);
}

Type Assembly::getType(const std::string &name) {
    std::string nameSpace;
    std::string className;

    auto pos = name.find_last_of('.');
    if (pos != std::string::npos) {
        nameSpace = name.substr(0, pos);
        className = name.substr(pos + 1);
    } else {
        className = name;
    }
    return getType(nameSpace, className);
}

std::vector<Type> Assembly::getTypes() {
    std::vector<Type> types;

    auto typeTable = mono_image_get_table_info(_image, MONO_TABLE_TYPEDEF);
    int numTypes = mono_table_info_get_rows(typeTable);
    for (int i = 0; i < numTypes; i++) {
        auto type = mono_class_get(_image, (i + 1) | MONO_TOKEN_TYPE_DEF);
        types.emplace_back(type);
    }
    return types;
}

Type Assembly::getType(const std::string &nameSpace, const std::string &name) {
    non_owning_ptr<MonoClass> type = mono_class_from_name(_image, nameSpace.c_str(), name.c_str());
    if (type == nullptr) {
        throw TypeLoadException("Type not found: " + nameSpace + "." + name);
    }
    return Type(type);
}