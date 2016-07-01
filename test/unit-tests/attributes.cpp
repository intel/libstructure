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

#include "attributes/Range.hpp"
#include "client/stock.hpp"
#include "client/type.hpp"
#include "importer/DefaultImporter.hpp"

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

TEST_CASE("Default attributes", "[structure][default]")
{
    auto root = Block("root", UInt8("a", attributes::Default(42)), Float("b"));

    CHECK_NOTHROW(root.with({defaultImporter, 3.14}));
    CHECK_NOTHROW(root.with({42, 3.14}));
    CHECK_THROWS(root.with({defaultImporter, defaultImporter}));
}