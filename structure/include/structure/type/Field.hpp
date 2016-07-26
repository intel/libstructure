/*
 * Copyright (c) 2016, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Intel Corporation nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include "structure/type/GenericField.hpp"
#include "structure/value/FieldValue.hpp"
#include "structure/value/GenericFieldValue.hpp"
#include "structure/ValueImporter.hpp"
#include "structure/Visitor.hpp"
#include "structure/Exception.hpp"

#include "structure/detail/safe_cast.hpp"

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
    FieldCrtp(const std::string &name, Attrs &&... attributes) : Base(name)
    {
        setAttributes(std::forward<Attrs>(attributes)...);
    };

    using Storage = _Storage;

    void accept(StructureVisitor &visitor) const override
    {
        visitor.visit(*static_cast<const Derived *>(this));
    }

    std::unique_ptr<GenericFieldValue> with(const std::string &value) const override
    {
        return std::make_unique<ThisValue>(*static_cast<const Derived *>(this), value);
    }
    bool isAllowed(const _Storage &) const { return true; };

    /** @returns A parsed value.
     *
     * @throws ParseError in case the value can't be parsed into the field's type.
     */
    static _Storage fromString(const std::string &input) { return safe_cast<_Storage>(input); }

    std::string getTypeName() const override { return Derived::typeToString(); }
    std::string getDescription() const override { return mAttributes.mDescription; }
    ValueImporter &getDefaultImporter() const override { return *(mAttributes.mDefaultImporter); }
    bool hasDefaultImporter() const override { return mAttributes.mHasDefaultImporter; }

    template <typename T>
    std::unique_ptr<GenericFieldValue> withTypedTemplate(T value) const try {
        return std::make_unique<ThisValue>(*static_cast<const Derived *>(this),
                                           safe_cast<_Storage>(value));
    } catch (CastError &e) {
        throw ParseError(this->getName() + ": " + e.what());
    }

    const Attributes &getAttributes() const { return mAttributes; }

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
