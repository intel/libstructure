#include "Visitor.hpp"
#include "Field.hpp"

namespace structure
{
// The various field types need to be forward-declared Visitor.hpp. As a result, the fact that they
// derive from GenericField is unknown by the compiler and so, the static_cast does not compile.
// This is why the definitions for these fallbacks need to be in their own cpp.
void StructureVisitor::visit(const Integer &i)
{
    visit(static_cast<const GenericField &>(i));
}
void StructureVisitor::visit(const Float &f)
{
    visit(static_cast<const GenericField &>(f));
}
}
