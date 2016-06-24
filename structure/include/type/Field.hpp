#pragma once

#include "type/GenericField.hpp"
#include "value/FieldValue.hpp"
#include "value/GenericFieldValue.hpp"
#include "ValueImporter.hpp"
#include "Exception.hpp"

#include "structure_export.h"

#include <safe_cast.hpp>

namespace structure
{
namespace detail
{
/** Helper class for implementing new field types.
 *
 * CRTP = Curiously Reccuring Template Pattern.
 */
template <typename Derived, typename Base, typename _Storage,
          typename Attributes = GenericFieldAttributes>
class FieldCrtp : public Base
{
    // This prevents hard-to-understand compilation errors.
    static_assert(is_structure<Base>::value, "Base must be a Structure.");

    static_assert(std::is_default_constructible<Attributes>::value,
                  "Attributes classes must be default-constructible.");

private:
    using This = FieldCrtp<Derived, Base, _Storage, Attributes>;
    using ThisValue = FieldValue<Derived>;

public:
    template <typename... Attrs>
    FieldCrtp(std::string name, Attrs &&... attributes) : Base(name)
    {
        setAttributes(std::forward<Attrs>(attributes)...);
    };

    using Storage = _Storage;

    std::unique_ptr<GenericFieldValue> with(const std::string &value) const override
    {
        return std::make_unique<ThisValue>(*static_cast<const Derived *>(this), value);
    }

    /** @returns A parsed value.
     *
     * @throws ParseError in case the value can't be parsed into the field's type.
     */
    static _Storage fromString(const std::string &input) { return safe_cast<_Storage>(input); }

    std::string getTypeName() const override { return Derived::typeToString(); }
    std::string getDescription() const override { return mAttributes.mDescription; }

    template <typename T>
    std::unique_ptr<GenericFieldValue> withTypedTemplate(T value) const try {
        return std::make_unique<ThisValue>(*static_cast<const Derived *>(this),
                                           safe_cast<_Storage>(value));
    } catch (CastError &e) {
        throw ParseError(this->getName() + ": " + e.what());
    }

private:
    Attributes mAttributes;

    template <typename Head, typename... Tail>
    void setAttributes(Head &&head, Tail &&... tail)
    {
        mAttributes.set(std::forward<Head>(head));
        setAttributes(std::forward<Tail>(tail)...);
    }
    void setAttributes() {}

    std::unique_ptr<StructureValue> doBuild(ValueImporter &importer,
                                            const std::string &path) const override
    {
        return importer.import(*this, path);
    }
};
} // namespace detail
}
