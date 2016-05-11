#pragma once

#include <list>
#include <memory>

#include "Structure.hpp"

namespace structure
{

class Block : public Structure
{

public:
    Block(std::string name);
    ~Block() override;

    void accept(Visitor &visitor) override;

    void addField(std::unique_ptr<Structure> &child);
    std::list<std::unique_ptr<Structure>> &getFields();

private:
    std::list<std::unique_ptr<Structure>> mFields;
};
}