#pragma once

#include "Structure.hpp"

#include <list>
#include <memory>

namespace structure
{

class Block : public Structure
{
public:
    Block(std::string name);

    void accept(Visitor &visitor) const override;

    void addField(std::unique_ptr<Structure> child);
    const std::list<std::unique_ptr<Structure>> &getFields() const;

private:
    std::list<std::unique_ptr<Structure>> mFields;
};
}
