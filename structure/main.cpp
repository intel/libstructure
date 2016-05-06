#include "Block.hpp"
#include "Field.hpp"
#include "functions.hpp"

int main(int argc, char **argv)
{
    structure::Block complex(
        "complex", {new structure::Field<float>{"re"}, new structure::Field<float>{"im"}});

    structure::display(complex);

    return 0;
}