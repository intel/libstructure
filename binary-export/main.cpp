#include "client/type.hpp"
#include "client/value.hpp"
#include "client/stock.hpp"
#include "functions.hpp"
#include "BinaryExport.hpp"

#include <iostream>

namespace strc = structure;

int main(void)
{
    auto root =
        strc::Block("MyData", strc::Block("Complex", strc::Float("Real"), strc::Float("Imaginary")),
                    strc::UInt32("Counter"));

    auto value = root.with({{"1.2", "3.4"}, "2"});

    binary_export::Visitor::Output out;
    binary_export::write(out, *value);

    std::cout.write((char *)out.data(), out.size());

    return 0;
}
