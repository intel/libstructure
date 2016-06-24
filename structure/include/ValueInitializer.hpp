#pragma once

#include "structure_export.h"
#include "ValueImporter.hpp"

#include <initializer_list>
#include <memory>

namespace structure
{

class GenericField;
class GenericFieldValue;

class STRUCTURE_EXPORT ValueInitializer : public ValueImporter
{
public:
    /** Constructs a ValueInitializer from a string literal
     *
     * It should coincide with a Field.
     */
    ValueInitializer(const char *atomicValue);

    /** Constructs a ValueInitializer from a long long literal
     */
    ValueInitializer(long long atomicValue, void *);

    /** Constructs a ValueInitializer from a unsigned long long literal
     */
    ValueInitializer(unsigned long long atomicValue, void *);

    /** Constructs a ValueInitializer from a double literal
     */
    ValueInitializer(double atomicValue, void *);

    /** Constructs a ValueInitializer from a signed integer
     */
    template <typename T>
    ValueInitializer(
        T atomicValue,
        typename std::enable_if<std::is_integral<T>::value and std::is_signed<T>::value, T>::type =
            0)
        : ValueInitializer(static_cast<long long>(atomicValue), nullptr)
    {
    }

    /** Constructs a ValueInitializer from an unsigned integer
     */
    template <typename T>
    ValueInitializer(T atomicValue,
                     typename std::enable_if<
                         std::is_integral<T>::value and std::is_unsigned<T>::value, T>::type = 0)
        : ValueInitializer(static_cast<unsigned long long>(atomicValue), nullptr)
    {
    }

    /** Constructs a ValueInitializer from a floating point
     */
    template <typename T>
    ValueInitializer(T atomicValue,
                     typename std::enable_if<std::is_floating_point<T>::value, T>::type = 0)
        : ValueInitializer(static_cast<double>(atomicValue), nullptr)
    {
    }

    /** Constructs a ValueInitializer from an previously-defined importer
     *
     * This should coincide with a Block.
     */
    ValueInitializer(ValueImporter &importer);

    /** Constructs a ValueInitializer from a recursive initializer list
     *
     * This should coincide with a Block.
     *
     * Example 1:
     * @code
     * PromptImporter<> importer;
     * Block structure("b1", Block("b2", ...), UInt8("u"), UInt8("v"));
     * structure.with({importer, "1", "2"});
     * @endcode
     *
     * Example 2:
     * @code
     * Block structure("b1", Block("b2", ...), UInt8("u"), UInt8("v"));
     * ValueInitializer value = {{"1", "2"}, "3", "4"};
     * structure.with(value);
     * @endcode
     */
    ValueInitializer(std::initializer_list<ValueInitializer> list);

    std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                              const std::string &path) override;
    void onEnterBlock(const std::string &block) override;
    void onExitBlock(const std::string &block) override;

private:
    // This class needs to be copiable because instances will be copied from initializer_list to
    // list when they are part of a ListImporter. (Copied and not moved because items of a
    // initializer_list can't be moved). That's why we use a shared_ptr.
    std::shared_ptr<ValueImporter> mImporter;
};
}
