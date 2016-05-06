#pragma once

#include "Structure.hpp"
#include "GenericField.hpp"

namespace structure
{

template <typename T>
class Field : public Structure, public GenericField
{

public:
    Field(std::string name);
    ~Field() override;

    void accept(Visitor *visitor) override;
    std::string getName() { return name(); }

private:
};
}

#include "field.tpp"