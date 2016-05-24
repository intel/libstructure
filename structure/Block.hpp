#pragma once

#include "Structure.hpp"
#include "StructureValue.hpp"
#include "ValueBuilder.hpp"

#include <list>
#include <memory>

namespace structure
{

class Block : public Structure
{
public:
    template <typename... Fields>
    Block(std::string name, Fields... fields) : Structure(name)
    {
        addFields(std::move(fields)...);
    }

    void accept(StructureVisitor &visitor) const override;

    const std::list<std::unique_ptr<Structure>> &getFields() const;

    std::unique_ptr<StructureValue> with(ValueBuilder builder) const override;
    std::string getTypeName() const override { return "Block"; }

private:
    std::list<std::unique_ptr<Structure>> mFields;

    void addField(std::unique_ptr<Structure> child);

    template <typename T, typename... Fields>
    void addFields(T first, Fields... fields)
    {
        addField(std::move(first));
        addFields(std::move(fields)...);
    }
    void addFields(){};
};
}
