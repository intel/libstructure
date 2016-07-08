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
#include "type/PrefixedArray.hpp"
#include <safe_cast.hpp>

#include <string>

using namespace structure;

TEST_CASE("Structure and Value creation", "[structure][value]")
{
    CHECK_NOTHROW(Block("name"));
    CHECK_NOTHROW(Float("name"));
    CHECK_NOTHROW(Double("name"));
    CHECK_NOTHROW(UInt8("name"));
    CHECK_NOTHROW(Int8("name"));
    CHECK_NOTHROW(UInt16("name"));
    CHECK_NOTHROW(Int16("name"));
    CHECK_NOTHROW(UInt32("name"));
    CHECK_NOTHROW(Int32("name"));
    CHECK_NOTHROW(UInt64("name"));
    CHECK_NOTHROW(Int64("name"));
    CHECK_NOTHROW(Q16f15("name"));
    CHECK_NOTHROW(String("name"));
    CHECK_NOTHROW(VarArray("name", UInt8("item name")));
    CHECK_NOTHROW(VarArray("name", Block("item name", UInt8("subitem name"))));
    CHECK_NOTHROW(PrefixedArray<UInt8>("name", UInt8("subitem name")));
    CHECK_NOTHROW(Array("name", UInt8("item name"), 2));

    CHECK_NOTHROW(Block("name").with({}));
    CHECK_NOTHROW(Float("name").with("42"));
    CHECK_NOTHROW(Double("name").with("42"));
    CHECK_NOTHROW(UInt8("name").with("42"));
    CHECK_NOTHROW(Int8("name").with("42"));
    CHECK_NOTHROW(UInt16("name").with("42"));
    CHECK_NOTHROW(Int16("name").with("42"));
    CHECK_NOTHROW(UInt32("name").with("42"));
    CHECK_NOTHROW(Int32("name").with("42"));
    CHECK_NOTHROW(UInt64("name").with("42"));
    CHECK_NOTHROW(Int64("name").with("42"));
    CHECK_NOTHROW(Q16f15("name").with("0.2"));
    CHECK_NOTHROW(String("name").with("bacon and spam"));
    CHECK_NOTHROW(VarArray("name", UInt8("item name")).with({"42"}));
    CHECK_NOTHROW(VarArray("name", Block("item name", UInt8("subitem name"))).with({{"42"}}));
    CHECK_NOTHROW(PrefixedArray<UInt8>("name", UInt8("subitem name")).with({"2", {"10", "20"}}));
    CHECK_NOTHROW(Array("name", UInt8("item name"), 2).with({"1", "2"}));
}

TEST_CASE("GetName", "[structure][value][name]")
{
    CHECK(Block("name").getName() == "name");
    CHECK(Float("name").getName() == "name");
    CHECK(Int32("name").getName() == "name");

    CHECK(Block("name").with({})->getName() == "name");
    CHECK(Float("name").with("42")->getName() == "name");
    CHECK(Int32("name").with("42")->getName() == "name");
}

TEST_CASE("Integer basic tests", "[structure][integer]")
{
    CHECK(UInt32::fromString("5") == 5);
    CHECK_THROWS(Int8::fromString("128"));

    UInt32 u32("u32");
    CHECK(u32.getSize() == 32);
    CHECK(u32.getSignedness() == false);

    Int16 s16("s16");
    CHECK(s16.getSize() == 16);
    CHECK(s16.getSignedness() == true);
}

TEST_CASE("FloatingPoint basic tests", "[structure][floating]")
{
    CHECK(Double::fromString("5.1") == Approx(5.1));
    CHECK_THROWS(Float::fromString("99999999999999999999999999999999999999999999999999999999999"));
}

TEST_CASE("FixedQ basic tests", "[structure][fixedq]")
{
    CHECK(Q16f15::fromString("0.5") == 16384);
    /** @todo implement range-checking for FixedQ */
    /* CHECK_THROWS(Q16f15::fromString("100000")); */

    NewFixedQ<16, 10, true, int16_t> q16f10("q16f10");
    CHECK(q16f10.getSize() == 16);
    CHECK(q16f10.getFractional() == 10);
    CHECK(q16f10.getIntegral() == 5);
    CHECK(q16f10.getSignedness() == true);

    NewFixedQ<16, 10, false, uint16_t> uq16f10("uq16f10");
    CHECK(uq16f10.getSize() == 16);
    CHECK(uq16f10.getFractional() == 10);
    CHECK(uq16f10.getIntegral() == 6);
    CHECK(uq16f10.getSignedness() == false);
}

TEST_CASE("Get Child", "[child]")
{
    std::unique_ptr<Block> block(new Block("block", Float("child")));

    SECTION ("Structure") {
        CHECK_NOTHROW(getChild(block, "child"));
        CHECK_THROWS(getChild(block, "notchild"));

        CHECK_NOTHROW(getChild(*block, "child"));
        CHECK_THROWS(getChild(*block, "notchild"));
    }

    SECTION ("Value") {
        auto value = block->with({"3.14"});
        CHECK_NOTHROW(getChild(value, "child"));
        CHECK_THROWS(getChild(value, "notchild"));
    }
}

TEST_CASE("Get value", "[value]")
{
    SECTION ("Int32") {
        Int32 structure("structure");
        CHECK(structure.with("42")->getValue() == "42");
        CHECK(with(structure, "42")->getValue() == "42");
        CHECK(with(structure, 42)->getValue() == "42");
    }
    SECTION ("Float") {
        Float structure("structure");

        CHECK(safe_cast<float>(structure.with("3.14")->getValue()) == Approx(3.14));
        CHECK(safe_cast<float>(with(structure, "3.14")->getValue()) == Approx(3.14));
        CHECK(safe_cast<float>(with(structure, 3.14f)->getValue()) == Approx(3.14));
    }
    SECTION ("FixedQ") {
        Q16f15 structure("structure");
        CHECK(structure.with("0.5")->getValue() == "16384");
        CHECK(with(structure, "0.25")->getValue() == "8192");
        CHECK(with(structure, 4096)->getValue() == "4096");
    }
    SECTION ("String") {
        String structure("structure");
        CHECK(structure.with("egg bacon and \"spam\"")->getValue() ==
              // MSVC fails to parse this:
              // R"("egg bacon and \"spam\"")");
              "\"egg bacon and \\\"spam\\\"\"");
    }
}

SCENARIO("Apply", "[apply][value][structure]")
{
    Block root("b1", Float("c1"), Block("b2", Float("c2"), Float("c3"),
                                        Block("b3", Block("b4", Float("c4")), Block("b5"))));

    int blockCount;
    int fieldCount;

    auto onEnterBlock = [&](auto &) { blockCount++; };
    auto onEnterField = [&](auto &) { fieldCount++; };

    GIVEN ("A structure") {

        THEN ("Applying a lambda visitor with recursion should visit all the blocks and fields") {
            blockCount = 0;
            fieldCount = 0;

            apply(root, onEnterBlock, defaultBlockFunction, onEnterField, true);
            CHECK(blockCount == 5);
            CHECK(fieldCount == 4);
        }

        THEN ("Applying a lambda visitor without recursion should visit only the first or "
              "block or field") {
            blockCount = 0;
            fieldCount = 0;

            apply(root, onEnterBlock, defaultBlockFunction, onEnterField, false);
            CHECK(blockCount == 1);
            CHECK(fieldCount == 0);
        }
    }

    GIVEN ("A value") {

        auto value = root.with({"1", {"2", "3", {{"4"}, {}}}});

        THEN ("Applying a lambda visitor with recursion should visit all the blocks and fields") {

            blockCount = 0;
            fieldCount = 0;

            apply(root, onEnterBlock, defaultBlockValueFunction, onEnterField, true);
            CHECK(blockCount == 5);
            CHECK(fieldCount == 4);
        }

        THEN ("Applying a lambda visitor without recursion should visit only the first or "
              "block or field") {

            blockCount = 0;
            fieldCount = 0;

            apply(root, onEnterBlock, defaultBlockValueFunction, onEnterField, false);
            CHECK(blockCount == 1);
            CHECK(fieldCount == 0);
        }
    }
}

SCENARIO("Constructing erroneous values", "[value][failure]")
{
    auto root =
        Block("root", Int8("a"), Block("b", Int8("c")), VarArray("varArray", Int8("d")), Int8("e"));

    GIVEN ("A valid structure") {
        THEN ("Creating a value missing a field should throw.") {
            CHECK_THROWS(with(root, {"1", {"2"}, {}}));
        }
        THEN ("Creating a value with too many fields should throw.") {
            CHECK_THROWS(with(root, {"1", {"2"}, {}, "3", "4"}));
        }
        THEN ("Creating a misaligned value should throw.") {
            // "1" is nested albeit it shouldn't
            CHECK_THROWS(with(root, {{"1"}, {"2"}, {}, "3"}));
            // "3" should be nested in an array
            CHECK_THROWS(with(root, {"1", {"2"}, "3", "4"}));
        }
    }
}

TEST_CASE("Metadatas", "[metadata]")
{
    Block root("root");
    root.setMetadata("attr", "true");
    CHECK(root.getMetadata().size() == 1);
    root.setMetadata("attr", "false");
    CHECK(root.getMetadata().size() == 1);
    root.setMetadata("other", "other");
    CHECK(root.getMetadata().size() == 2);
}

TEST_CASE("BlockStructure", "[structure][block]")
{
    Block root("root", Float("f1"), Float("f2"));
    CHECK(root.getFields().size() == 2);
}

TEST_CASE("Typed With", "[typed][value]")
{
    Block root("MyData", UInt8("Counter"), Float("Double"), Q32f31("Fixed"), String("String"));
    CHECK_NOTHROW(auto value = root.with({255, 3.14f, 0.01, "bacon and spam"}));
    CHECK_NOTHROW(auto value = root.with({255, 3.14f, 0.01, ""}));
    CHECK_THROWS(auto value = root.with({-1, 3.14f, 0.01, "sausages"}));
    CHECK_THROWS(auto value = root.with({255, 3.14f, 12, "eggs"}));
    CHECK_THROWS(auto value = root.with({256, 3.14f, 0.01, "spam"}));
    CHECK_THROWS(auto value = root.with({255, 3.14f, 0.01, 42}));
}
