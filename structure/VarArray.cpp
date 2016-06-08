#include "type/VarArray.hpp"
#include "Exception.hpp"

namespace structure
{
BlockValue VarArray::with(ValueBuilder builder) const
{
    if (builder.atom) {
        throw ValueStructureMismatch(getName());
    }

    BlockValue b(*this);
    for (auto &entry : builder.listValue) {
        // A VarArray type only contains one field but a value can contain
        // multiple instances of that field type.
        b.addValue(getFields()[0].get().with(entry));
    }
    return b;
}

std::string VarArray::getTypeName() const
{
    return "VarArray";
}
};
