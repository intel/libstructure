#pragma once

#include "Visitor.hpp"
#include "client/value.hpp"

#include <vector>
#include <iterator>

#ifdef _MSC_VER
/** Visual studio raises a warning if the check iterator feature is activated
 * but a raw pointer is used as iterator (as it can not check it's bounds).
 * As it is a safety feature, do not silent the warning, but use the
 * microsoft specific `make_check_array_iterator` that take a pointer
 * and the size of the underline buffer.
 * For other compiler, use the raw pointer.
 */
#define MAKE_ARRAY_ITERATOR(begin, size) stdext::make_checked_array_iterator(begin, size)
#else
/** By default an array iterator is a pointer to the first element. */
#define MAKE_ARRAY_ITERATOR(begin, size) begin
#endif

namespace binary_export
{
class Visitor : public structure::StorageVisitor
{
public:
    using Output = std::vector<unsigned char>;
    Visitor(Output &out) : mOut(out) {}

    void visitStorage(unsigned char v) override { visitStorageT(v); }
    void visitStorage(signed char v) override { visitStorageT(v); }
    void visitStorage(char v) override { visitStorageT(v); }
    void visitStorage(unsigned short v) override { visitStorageT(v); }
    void visitStorage(signed short v) override { visitStorageT(v); }
    void visitStorage(unsigned int v) override { visitStorageT(v); }
    void visitStorage(int v) override { visitStorageT(v); }
    void visitStorage(unsigned long v) override { visitStorageT(v); }
    void visitStorage(long v) override { visitStorageT(v); }
    void visitStorage(float v) override { visitStorageT(v); }
    void visitStorage(double v) override { visitStorageT(v); }
    void visitStorage(unsigned long long v) override { visitStorageT(v); }
    void visitStorage(long long v) override { visitStorageT(v); }
    void visitStorage(long double v) override { visitStorageT(v); }

private:
    template <class Storage>
    void visitStorageT(Storage v)
    {
        auto type_erased = reinterpret_cast<const Output::value_type *>(&v);
        auto first = MAKE_ARRAY_ITERATOR(type_erased, sizeof(v));
        std::copy(first, first + sizeof(v), std::back_inserter(mOut));
    }

    Output &mOut;
};

void write(Visitor::Output &out, const structure::StructureValue &value)
{
    Visitor visitor(out);
    value.accept(visitor);
}
} // namespace binary_export
