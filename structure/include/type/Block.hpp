#pragma once

#include "structure_export.h"

#include "type/Structure.hpp"
#include "value/BlockValue.hpp"
#include "value/StructureValue.hpp"
#include "ValueBuilder.hpp"

#include <vector>
#include <utility>
#include <memory>

namespace structure
{

class STRUCTURE_EXPORT Block : public Structure
{
public:
    template <typename... Fields>
    Block(std::string name, Fields &&... fields) : Structure(name)
    {
        addFields(std::forward<Fields>(fields)...);
    }

    Block(Block &&other) = default;
    Block &operator=(Block &&other) = default;

    void accept(StructureVisitor &visitor) const override;

    using StructureRef = std::reference_wrapper<const Structure>;
    virtual std::vector<StructureRef> getFields() const;

    BlockValue with(ValueBuilder builder) const;
    std::string getTypeName() const override { return "Block"; }

private:
    std::vector<std::unique_ptr<Structure>> mFields;

    std::unique_ptr<StructureValue> genericWith(ValueBuilder builder) const override;
    template <class T>
    void addField(T &&child)
    {
        mFields.emplace_back(new T(std::forward<T>(child)));
    }

    template <typename T, typename... Fields>
    void addFields(T &&first, Fields &&... fields)
    {
        addField(std::forward<T>(first));
        addFields(std::forward<Fields>(fields)...);
    }

    void addFields(){};
};
}
