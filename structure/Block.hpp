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

    void accept(Visitor &visitor) override;

    void addField(std::unique_ptr<Structure> child);
    const std::list<std::unique_ptr<Structure>> &getFields();

private:
    std::list<std::unique_ptr<Structure>> mFields;
};
}
