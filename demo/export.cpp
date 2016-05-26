#include "export.hpp"
#include "functions.hpp"
#include "Field.hpp"

#include <sstream>
#include <map>
#include <string>

namespace structure
{

std::string exportStructure(const Structure &structure, std::list<std::string> &components,
                            int level = 1)
{
    auto tab = [&](int level) { return std::string(4 * level, ' '); };

    std::string instance;

    auto onEnterBlock = [&](auto &block) {
        std::stringstream result;

        result << tab(1) << "<ComponentType Name=\"" << block.getName() << "\">\n";

        for (auto &field : block.getFields()) {
            result << exportStructure(*field, components, 2) << "\n";
        }

        result << tab(1) << "</ComponentType>\n";

        components.push_back(result.str());

        instance = tab(level) + "<Component Name=\"" + block.getName() + "\" Type=\"" +
                   block.getName() + "\">";
    };

    auto onEnterField = [&](auto &field) {
        instance = tab(level) + "<" + field.getTypeName() + " Name=\"" + field.getName() + "\">";
    };

    apply(structure, onEnterBlock, defaultBlockFunction, onEnterField, false);

    return instance;
}

std::string exportStructure(const Structure &structure)
{
    std::stringstream result;

    std::list<std::string> components;

    std::string instance = exportStructure(structure, components);

    result << "<ComponentLibrary>\n\n";

    for (auto &c : components) {
        result << c << "\n";
    }

    result << "</ComponentLibrary>\n\n";

    result << "<InstanceDefinition>\n\n";

    result << instance << "\n\n";

    result << "</InstanceDefinition>\n";

    return result.str();
}
}