#pragma once

#include <list>
#include "Structure.hpp"

namespace structure
{

class Block : public Structure
{

public:
    Block(std::string name, std::initializer_list<Structure *> fields);
    ~Block() override;

    void accept(Visitor *visitor) override;

    std::list<Structure *> getFields();

private:
    std::list<Structure *> mFields;
};
}