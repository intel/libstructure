#pragma once

#include "ValueImporter.hpp"
#include "type/GenericField.hpp"

#include <iostream>

namespace structure
{

/** An implementation of a ValueImporter that gets its values from a stream
 *
 * @tparam     Input : A class that must provide an overload of the operator >>(std::string&)
 *
 * Example of custom class :
 * @code
 * class MyStream
 * {
 * public:
 *     MyStream &operator>>(std::string &s)
 *     {
 *         s = "42";
 *         return *this;
 *     }
 * };
 * @endcode
 *
 * By default, the StreamImporter will use std::cin, so
 * @code
 * StreamImporter<> importer();
 * @endcode
 * is equivalent to
 * @code
 * StreamImporter<std::istream> importer(std::cin);
 * @endcode
 *
 * @ingroup ValueImporter
 *
 */
template <class Input = std::istream>
class StreamImporter : public ValueImporter
{

public:
    StreamImporter(Input &input = std::cin) : mInput(input) {}

    std::unique_ptr<GenericFieldValue> import(const GenericField &f,
                                              const std::string & /*path*/) override
    {
        std::string val = read();
        return f.with(val.c_str());
    }

private:
    std::string read()
    {
        std::string result;
        mInput >> result;
        return result;
    }

    Input &mInput;
};
} // namespace structure