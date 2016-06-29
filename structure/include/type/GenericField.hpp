#pragma once

#include "structure_export.h"

#include "Structure.hpp"
#include "Exception.hpp"
#include "value/GenericFieldValue.hpp"
#include "attributes/Default.hpp"

namespace structure
{

struct STRUCTURE_EXPORT GenericFieldAttributes : StructureAttributes
{
    void set(const attributes::Default &def)
    {
        mDefaultImporter = def.mDefaultImporter;
        mHasDefaultImporter = true;
    }

    template <typename C>
    void set(const C &c)
    {
        StructureAttributes::set(c);
    }

    bool mHasDefaultImporter = false;
    std::shared_ptr<ValueImporter> mDefaultImporter;
};

/** Base class for all atomic field types. */
class STRUCTURE_EXPORT GenericField : public Structure
{
public:
    using Structure::Structure;
    virtual ~GenericField() = default;

    /** Creates a value from the given string
     */
    virtual std::unique_ptr<GenericFieldValue> with(const std::string &value) const = 0;

    /** Creates a value from the given typed value
    */
    template <typename T,
              typename = typename std::enable_if<not std::is_same<T, std::string>::value and
                                                 not std::is_same<T, const char *>::value>::type>
    std::unique_ptr<GenericFieldValue> with(T value) const
    {
        return withTyped(value);
    }

    virtual ValueImporter &getDefaultImporter() const = 0;
    virtual bool hasDefaultImporter() const = 0;

private:
    /** Creates a value from the given long long
     */
    virtual std::unique_ptr<GenericFieldValue> withTyped(long long) const
    {
        throw ParseError(getName());
    }
    /** Creates a value from the given unsigned long long
     */
    virtual std::unique_ptr<GenericFieldValue> withTyped(unsigned long long) const
    {
        throw ParseError(getName());
    }
    /** Creates a value from the given float
     */
    virtual std::unique_ptr<GenericFieldValue> withTyped(float) const
    {
        throw ParseError(getName());
    }
    /** Creates a value from the given double
     */
    virtual std::unique_ptr<GenericFieldValue> withTyped(double) const
    {
        throw ParseError(getName());
    }
};
}
