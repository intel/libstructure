#include <catch.hpp>

#include "client/type.hpp"
#include "client/value.hpp"
#include "client/stock.hpp"
#include "BinaryExport.hpp"

namespace structure
{
SCENARIO("Binary export", "[export][value][binary]")
{
    binary_export::Visitor::Output actual;
    GIVEN ("A simple 8-bit unsigned integer value") {
        UInt8 type("u8");
        auto value = type.with("0xab");
        binary_export::Visitor::Output expected = {0xab};

        THEN ("Serializing it should produce a 1-byte-long result.") {
            binary_export::write(actual, value);
            CHECK(actual == expected);
        }
    }

    GIVEN ("A nested strucure with various types") {
        auto type = Block("block1", Block("block2", Float("float1"), Int16("i16")), UInt32("u32"));
        auto value = type.with({{"1.23", "-16000"}, "320000032"});
        binary_export::Visitor::Output expected = {
            0xa4, 0x70, 0x9d, 0x3f, // 1.23
            0x80, 0xc1,             // -16000
            0x20, 0xd0, 0x12, 0x13, // 320000032
        };

        THEN ("Serializing it should produce a flatten string of bytes.") {
            binary_export::write(actual, value);
            CHECK(actual == expected);
        }
    }
}
} // namespace structure
