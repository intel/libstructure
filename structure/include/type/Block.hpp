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

    // Specialized block can override this method; it will be called by Block::genericWith
    virtual BlockValue with(ValueBuilder builder) const;
    std::string getTypeName() const override { return "Block"; }

protected:
    template <typename T, typename... Fields>
    void addFields(T &&first, Fields &&... fields)
    {
        addField(std::forward<T>(first));
        addFields(std::forward<Fields>(fields)...);
    }

private:
    std::unique_ptr<StructureValue> genericWith(ValueBuilder builder) const override;

    template <class T>
    void addField(T &&child)
    {
        // This prevents hard-to-understand compilation errors.
        static_assert(is_structure<T>::value, "The field to be added must be a Structure.");
        mFields.emplace_back(new T(std::forward<T>(child)));
    }

    void addFields(){};

    std::vector<std::unique_ptr<Structure>> mFields;
};
}
