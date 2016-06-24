#include <catch.hpp>

#include "attributes/Range.hpp"
#include "client/stock.hpp"

using namespace structure;

TEST_CASE("Range attribute", "[attributes][range]")
{
    using attributes::mkRange;
    using attributes::Range;

    auto autoRange = mkRange(-10, 10);
    // This range can fit in an int8...
    Range<int8_t> i8Range = autoRange;
    // ...and then in an int32.
    Range<int32_t> i32Range = i8Range;
    // It can also be safely downcast into an int16...
    Range<int16_t> i16Range(i32Range);
    // ...but not in a uint8.
    CHECK_THROWS(Range<uint8_t> u8Range = autoRange);

    // Creating an empty range (min > max) is illegal
    CHECK_THROWS(mkRange(20, 10));

    CHECK(Range<uint8_t>::Min(10) == Range<uint8_t>(10, 255));
    CHECK(Range<uint8_t>::Max(10) == Range<uint8_t>(0, 10));
}

TEST_CASE("Type attributes", "[structure][attributes]")
{
    // Attributes are optional and empty by default.
    CHECK(UInt8("name").getDescription() == "");

    std::string desc("Some integer");
    auto range = attributes::mkRange(-10, 10);
    Int8 i8("name", attributes::Description{desc}, range);

    CHECK(i8.getDescription() == desc);

    CHECK_NOTHROW(i8.with("10"));
    CHECK_NOTHROW(i8.with("-10"));
    CHECK_THROWS(i8.with("11"));
    CHECK_THROWS(i8.with("-11"));

    auto rangeStr = attributes::mkRange("3", "3");
    UInt16 u16("", rangeStr);
    CHECK_NOTHROW(u16.with("3"));
    CHECK_THROWS(u16.with("4"));
}
