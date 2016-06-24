#pragma once

#include "structure_export.h"

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
 * name (the name with which it is referred to) and a type name (the type of structure it is);
 * see examples below. A Structure may also have arbitrary metadata. The definition of this
 * metadata belongs to the user and constitutes a protocol agreement between the producer and the
 * consumer of the structure. A Structured Value (structure::StructureValue) can be created based on
 * a Structure.
 *
 * To better understand what the name and the type name are, consider the following C code:
 *
 * @code
 * struct MyStruct {
 *     int foo;
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
    Structure(std::string name);
    virtual ~Structure() = default;

    /** Entry point for Structure visitors.
     *
     * This implements the "Visitor" design pattern.
     */
    virtual void accept(StructureVisitor &visitor) const = 0;

    /** Return the structure's name; see the main description. */
    std::string getName() const;
    /** Return the structure's type name; see the main description */
    virtual std::string getTypeName() const = 0;

    /** Set an arbitrary metadata. */
    void setMetadata(std::string key, std::string value = "");
    /** Get the map of metadata. */
    const std::map<std::string, std::string> &getMetadata();

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
}
