#include "functions.hpp"
#include "type/StockTypes.hpp"
#include "type/Integer.hpp"
#include "type/FloatingPoint.hpp"
#include "ValueBuilder.hpp"
#include "export.hpp"

#include <iostream>
#include <exception>

namespace strc = structure;

int main(void)
{

    auto root =
        strc::Block("MyData", strc::Block("Complex", strc::Float("Real"), strc::Float("Imaginary")),
                    strc::UInt32("Counter"));

    strc::display(root);

    std::cout << std::endl;

    strc::UInt32 int32Type("SomeU32");
    auto valueFromInt = with(int32Type, 25);
    auto valueFromString = with(int32Type, "25");
    strc::display(valueFromInt);
    strc::display(valueFromString);

    strc::Float floatType("SomeFloat");
    auto valueFromMethod = with(floatType, 1.2);
    auto valueFromFreefunc = floatType.with("1.2");
    strc::display(valueFromMethod);
    strc::display(valueFromFreefunc);

    auto r = strc::with(root, {{"1.2", "3.4"}, "2"});

    strc::display(r);

    std::cout << std::endl;

    std::cout << exportStructure(root) << std::endl;

    return 0;
}
