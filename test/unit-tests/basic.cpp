#include <catch.hpp>

#include "functions.hpp"
#include "client/stock.hpp"
#include "client/type.hpp"
#include "client/value.hpp"
#include "convert.hpp"

#include <sstream>
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

    CHECK_NOTHROW(Block("name").with({}));
    CHECK_NOTHROW(Float("name").with("42"));
    CHECK_NOTHROW(Double("name").with("42"));
    CHECK_NOTHROW(UInt8("name").with("42"));
    CHECK_NOTHROW(Int8("name").with("42"));
    CHECK_NOTHROW(UInt16("name").with("42"));
    CHECK_NOTHROW(Int16("name").with("42"));
    CHECK_NOTHROW(UInt32("name").with("42"));
    CHECK_NOTHROW(Int32("name").with("42"));
}

TEST_CASE("GetName", "[structure][value][name]")
{
    CHECK(Block("name").getName() == "name");
    CHECK(Float("name").getName() == "name");
    CHECK(Int32("name").getName() == "name");

    CHECK(Block("name").with({}).getName() == "name");
    CHECK(Float("name").with("42").getName() == "name");
    CHECK(Int32("name").with("42").getName() == "name");
}

TEST_CASE("Get Child", "[child]")
{
    SECTION ("Structure") {
        Block block("block", Float("child"));
        CHECK_NOTHROW(getChild(block, "child"));
        CHECK_THROWS(getChild(block, "notchild"));
    }

    SECTION ("Value") {
        Block block("block", Float("child"));
        auto value = block.with({"3.14"});
        CHECK_NOTHROW(getChild(value, "child"));
        CHECK_THROWS(getChild(value, "notchild"));
    }
}

TEST_CASE("Get value", "[value]")
{
    SECTION ("Int32") {
        Int32 structure("structure");
        CHECK(structure.with("42").getValue() == "42");
        CHECK(with(structure, "42").getValue() == "42");
        CHECK(with(structure, 42).getValue() == "42");
    }
    SECTION ("Float") {
        Float structure("structure");
        float f;

        CHECK(convertTo(structure.with("3.14").getValue(), f));
        CHECK(f == Approx(3.14f));

        CHECK(convertTo(with(structure, "3.14").getValue(), f));
        CHECK(f == Approx(3.14f));

        CHECK(convertTo(with(structure, 3.14f).getValue(), f));
        CHECK(f == Approx(3.14f));
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

TEST_CASE("Value Builder", "[value]")
{
    SECTION ("AtomicValue") {
        ValueBuilder builder("atom");
        CHECK(builder.atom == true);
        CHECK(builder.atomicValue == "atom");
    }

    SECTION ("ListValue") {
        ValueBuilder builder({"atom1", "atom2"});
        CHECK(builder.atom == false);
        CHECK(builder.listValue.size() == 2);
    }
}

TEST_CASE("Attributes", "[attributes]")
{
    Block root("root");
    root.setAttribute("attr", "true");
    CHECK(root.getAttributes().size() == 1);
    root.setAttribute("attr", "false");
    CHECK(root.getAttributes().size() == 1);
    root.setAttribute("other", "other");
    CHECK(root.getAttributes().size() == 2);
}

TEST_CASE("BlockStructure", "[structure][block]")
{
    Block root("root", Float("f1"), Float("f2"));
    CHECK(root.getFields().size() == 2);
}

TEST_CASE("Display", "[structure][value][display]")
{
    std::unique_ptr<Block> root(new Block("root", Float("a"), Int32("b")));

    SECTION ("Structure") {
        std::stringstream ss;

        std::string expected = "Block : root {\n"
                               "    FloatingPoint : a\n"
                               "    Integer : b\n"
                               "}\n";

        print(ss, root);
        CHECK(ss.str() == expected);
    }

    SECTION ("Value") {
        auto value = root->with({"1", "2"});
        std::stringstream ss;

        std::string expected = "BlockValue : root {\n"
                               "    FloatingPoint : a = 1.000000\n"
                               "    Integer : b = 2\n"
                               "}\n";

        print(ss, value);
        CHECK(ss.str() == expected);
    }
}