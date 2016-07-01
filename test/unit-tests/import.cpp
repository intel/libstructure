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

#include "importer/StreamImporter.hpp"
#include "importer/PromptImporter.hpp"
#include "importer/MapImporter.hpp"

#include "functions.hpp"
#include "client/stock.hpp"
#include "client/type.hpp"
#include "client/value.hpp"

#include <sstream>
#include <map>

using namespace structure;

SCENARIO("Importing values with ValueImporter subclasses", "[value][import]")
{
    GIVEN ("A valid structure ") {
        auto root =
            Block("root", Int8("a"), Block("b", Float("c"), Int8("d")), String("e"), Int8("f"));
        std::unique_ptr<StructureValue> value;
        std::stringstream null;
        std::stringstream ss;

        GIVEN ("A StreamImporter") {
            auto importer = StreamImporter<>(ss);
            THEN ("Creating a value from correct input should not throw.") {
                ss << R"(1 2.3 4 "egg and spam" 5)";
                CHECK_NOTHROW(value = build(root, importer));
                CHECK(getValue(value) == R"({1, {2.300000, 4}, "egg and spam", 5})");
            }
            THEN ("Creating a value from erroneous input should throw.") {
                ss << R"(1.2 3 4 "egg and spam" 5)";
                CHECK_THROWS(build(root, importer));
            }
            THEN ("Creating a value from partial input should throw.") {
                ss << "1 2.3";
                CHECK_THROWS(build(root, importer));
            }
        }

        GIVEN ("A PromptImporter") {
            auto importer = PromptImporter<>(ss, null);
            THEN ("Creating a value from correct input should not throw.") {
                ss << R"(1 2.3 4 "egg and spam" 5)";
                CHECK_NOTHROW(value = build(root, importer));
                CHECK(getValue(value) == R"({1, {2.300000, 4}, "egg and spam", 5})");
            }
            THEN ("Creating a value from erroneous input should throw.") {
                ss << R"(1.0 2.3 4 "egg and spam")";
                CHECK_THROWS(build(root, importer));
            }
            THEN ("Creating a value from partial input should throw.") {
                ss << "1 2.3";
                CHECK_THROWS(build(root, importer));
            }
        }

        GIVEN ("A MapImporter") {
            THEN ("Creating a value from correct input should not throw.") {
                std::map<std::string, std::string> values = {{"/root/a", "1"},
                                                             {"/root/b/c", "2.3"},
                                                             {"/root/b/d", "4"},
                                                             {"/root/e", "egg and spam"},
                                                             {"/root/f", "5"}};
                auto importer = MapImporter(values);
                CHECK_NOTHROW(value = build(root, importer));
                CHECK(getValue(value) == R"({1, {2.300000, 4}, "egg and spam", 5})");
            }
            THEN ("Creating a value from erroneous input should throw.") {
                std::map<std::string, std::string> values = {
                    {"/root/a", "1.0"}, {"/root/b/c", "2.3"}, {"/root/d", "4"},
                };
                auto importer = MapImporter(values);
                CHECK_THROWS(build(root, importer));
            }
            THEN ("Creating a value from partial input should throw.") {
                std::map<std::string, std::string> values = {
                    {"/root/a", "1"}, {"/root/b/c", "2.3"},
                };
                auto importer = MapImporter(values);
                CHECK_THROWS(build(root, importer));
            }
        }
        GIVEN ("A custom Importer") {
            THEN ("onEnterBlock and onExitBlock should be called for each block") {
                struct CustomImporter : StreamImporter<>
                {
                    int onEnterBlockCount = 0;
                    int onExitBlockCount = 0;

                    CustomImporter(std::istream &input) : StreamImporter<>(input) {}

                    void onEnterBlock(const std::string &) override { onEnterBlockCount++; }

                    void onExitBlock(const std::string &) override { onExitBlockCount++; }
                };

                ss << R"(1 2.3 4 "" 5)";
                CustomImporter importer(ss);
                CHECK_NOTHROW(value = build(root, importer));
                CHECK(getValue(value) == R"({1, {2.300000, 4}, "", 5})");
                CHECK(importer.onEnterBlockCount == 2);
                CHECK(importer.onExitBlockCount == 2);
            }
        }

        GIVEN ("Mixed literal and stream importers") {
            StreamImporter<> importer(ss);
            THEN ("Creating a value from correct input should work") {
                ss << "2.3 4";
                CHECK_NOTHROW(value = root.with({"1", importer, "egg and spam", "5"}));
                CHECK(getValue(value) == R"({1, {2.300000, 4}, "egg and spam", 5})");
            }
        }
    }
}
