/*
 * Copyright (c) 2016, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <catch.hpp>

#include "functions.hpp"
#include "client/stock.hpp"
#include "type/helper/PrefixedArray.hpp"
#include "client/type.hpp"
#include "client/value.hpp"

using namespace structure;

TEST_CASE("Display", "[structure][value][display]")
{
    Block root("root", Float("a"), Int32("b"), Q16f15("c"), VarArray("varArray", UInt8("d")),
               PrefixedArray<UInt8>("lv", Block("block", UInt16("x"), UInt16("y"))));

    SECTION ("Structure") {
        std::stringstream ss;

        std::string expected = "Block : root {\n"
                               "    Float : a\n"
                               "    Int32 : b\n"
                               "    Q16f15 : c\n"
                               "    VarArray : varArray {\n"
                               "        UInt8 : d\n"
                               "    }\n"
                               "    LV (UInt8) : lv {\n"
                               "        Block : block {\n"
                               "            UInt16 : x\n"
                               "            UInt16 : y\n"
                               "        }\n"
                               "    }\n"
                               "}\n";

        print(ss, root);
        CHECK(ss.str() == expected);
    }

    SECTION ("Value") {
        auto value = root.with({"1", "2", "0.5", {"3", "4"}, {"2", {{"1", "2"}, {"3", "4"}}}});
        std::stringstream ss;

        std::string expected = "BlockValue : root {\n"
                               "    Float : a = 1.000000\n"
                               "    Int32 : b = 2\n"
                               "    Q16f15 : c = 16384\n"
                               "    BlockValue : varArray {\n"
                               "        UInt8 : d = 3\n"
                               "        UInt8 : d = 4\n"
                               "    }\n"
                               "    BlockValue : lv {\n"
                               "        UInt8 : count = 2\n"
                               "        BlockValue : lv {\n"
                               "            BlockValue : block {\n"
                               "                UInt16 : x = 1\n"
                               "                UInt16 : y = 2\n"
                               "            }\n"
                               "            BlockValue : block {\n"
                               "                UInt16 : x = 3\n"
                               "                UInt16 : y = 4\n"
                               "            }\n"
                               "        }\n"
                               "    }\n"
                               "}\n";

        print(ss, value);
        CHECK(ss.str() == expected);
    }
}
