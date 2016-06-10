#pragma once

#include "ValueImporter.hpp"

namespace structure
{

/** An implementation of a ValueImporter that prompt for the values
 *
 * @tparam     Input  : A class that must provide an overload of the operator >>(std::string&)
 * @tparam     Output : A class that must provide an overload of the operator <<(std::string&)
 *
 *
 * By default, the PromptImporter will use std::cin as input and std::cout as output, so
 * @code
 * PromptImporter<> importer();
 * @endcode
 * is equivalent to
 * @code
 * PromptImporter<std::istream, std::ostream> importer(std::cin, std::count);
 * @endcode
 *
 * @ingroup ValueImporter
 *
 */
template <class Input = std::istream, class Output = std::ostream>
class PromptImporter : public ValueImporter
{

public:
    PromptImporter(Input &input = std::cin, Output &output = std::cout)
        : mInput(input), mOutput(output)
    {
    }

    std::unique_ptr<StructureValue> import(const GenericField &f, const std::string &path) override
    {
        std::string val = read(path + " = ");
        return f.with(val.c_str());
    }

private:
    std::string read(const std::string &message)
    {
        mOutput << message;
        std::string result;
        mInput >> result;
        return result;
    }

    Input &mInput;
    Output &mOutput;
};
} // namespace structure