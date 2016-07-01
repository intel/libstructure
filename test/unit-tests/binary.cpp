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
            binary_export::write(actual, *value);
            CHECK(actual == expected);
        }
    }

    GIVEN ("A string") {
        String s("");
        auto value = s.with("spam");
        binary_export::Visitor::Output expected = {'s', 'p', 'a', 'm', '\0'};

        THEN ("Serializing it should produce a null-terminated string.") {
            binary_export::write(actual, *value);
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
            binary_export::write(actual, *value);
            CHECK(actual == expected);
        }
    }
}
} // namespace structure
