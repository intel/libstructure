/*
 * Copyright (c) 2016, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include "structure/structure_export.h"

#include "structure/attributes/Description.hpp"

#include <initializer_list>
#include <string>
#include <map>
#include <memory>

namespace structure
{

class StructureValue;
class StructureVisitor;
class ValueImporter;

/** Base class for all structure types.
 *
 * This class represents any structure (field, block or combination of those). A Structure has a
 * type name (the type of structure it is); it also has well-known attributes, such as a
 * description. See examples below. A Structure may also have arbitrary (free-form) metadata. The
 * definition of this metadata belongs to the user and constitutes a protocol agreement between
 * the producer and the consumer of the structure. A Structured Value (structure::StructureValue)
 * can be created based on a Structure.
 *
 * To better understand what the name and the type name are, consider the following C code:
 *
 * @code
 * struct MyStruct {
 *     int foo; //< This is a Description (a well-known attributes).
 *     char bar;
 * };
 * @endcode
 *
 * The name of the `MyStruct` struct is "MyStruct" and its type name is "struct" (note that
 * libstructure will call it a "Block" instead). `foo`'s name is "foo" and its type name is "int".
 */
class STRUCTURE_EXPORT Structure
{
public:
    Structure(const std::string &name) : mName(name){};
    virtual ~Structure() = default;

    /** Entry point for Structure visitors.
     *
     * This implements the "Visitor" design pattern.
     */
    virtual void accept(StructureVisitor &visitor) const = 0;

    /** Return the structure's name; see the main description. */
    std::string getName() const;
    /** Return the structure's description; see the main description. */
    virtual std::string getDescription() const = 0;
    /** Return the structure's type name; see the main description */
    virtual std::string getTypeName() const = 0;

    /** Set an arbitrary metadata. */
    void setMetadata(const std::string &key, const std::string &value = "");
    /** Get the map of metadata. */
    const std::map<std::string, std::string> &getMetadata() const;

    /** Create a StructureValue from a value importer
     *
     * Usage exemple:
     * @code
     * std::stringstream values;
     * values << "1 2 3 4";
     * auto importer = StreamImporter<>(values);
     *
     * Block myBlock(UInt8("a"), UInt8("d"), UInt8("c"), UInt8("b"));
     * auto myValue = myBlock.with(importer);
     * @endcode
     *
     * @param importer: The source of values
     * @param path: The path of the field to be imported. Internal context - users of the Structure
     *              class should not override the default argument value.
     */
    std::unique_ptr<StructureValue> build(ValueImporter &importer,
                                          const std::string &path = "") const;

private:
    virtual std::unique_ptr<StructureValue> doBuild(ValueImporter &importer,
                                                    const std::string &path) const = 0;
    std::string mName;
    std::map<std::string, std::string> mMetadata;
};

/** Helper type trait for checking that a type is a Structure
 *
 * It helps preventing complicated error messages further down the stack.
 *
 * @tparam T The type to be checked against Structure.
 */
template <typename T>
struct is_structure : std::is_base_of<Structure, T>
{
};

/** Base class for all attribute classes
 *
 * All fields must have these attributes. For now, it only contains a description attribute.
 */
struct STRUCTURE_EXPORT StructureAttributes
{
    void set(const attributes::Description &desc) { mDescription = desc.mValue; }

    std::string mDescription;
};
}
