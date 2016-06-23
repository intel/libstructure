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
        auto root = Block("root", Int8("a"), Block("b", Float("c"), Int8("d")), Int8("e"));
        std::stringstream null;
        std::stringstream ss;

        GIVEN ("A StreamImporter") {
            THEN ("Creating a value from correct input should not throw.") {
                ss << "1 2.3 4 5";
                auto importer = StreamImporter<>(ss);
                std::unique_ptr<StructureValue> value;
                CHECK_NOTHROW(value = build(root, importer));
                CHECK(getValue(value) == "{1, {2.300000, 4}, 5}");
            }
            THEN ("Creating a value from erroneous input should throw.") {
                ss << "1.2 3 3";
                auto importer = StreamImporter<>(ss);
                CHECK_THROWS(build(root, importer));
            }
            THEN ("Creating a value from partial input should throw.") {
                ss << "1 2.3";
                auto importer = StreamImporter<>(ss);
                CHECK_THROWS(build(root, importer));
            }
        }

        GIVEN ("A PromptImporter") {
            THEN ("Creating a value from correct input should not throw.") {
                ss << "1 2.3 4 5";
                auto importer = PromptImporter<>(ss, null);
                std::unique_ptr<StructureValue> value;
                CHECK_NOTHROW(value = build(root, importer));
                CHECK(getValue(value) == "{1, {2.300000, 4}, 5}");
            }
            THEN ("Creating a value from erroneous input should throw.") {
                ss << "1.0 2.3 4";
                auto importer = PromptImporter<>(ss, null);
                CHECK_THROWS(build(root, importer));
            }
            THEN ("Creating a value from partial input should throw.") {
                ss << "1 2.3";
                auto importer = PromptImporter<>(ss, null);
                CHECK_THROWS(build(root, importer));
            }
        }

        GIVEN ("A MapImporter") {
            THEN ("Creating a value from correct input should not throw.") {
                std::map<std::string, std::string> values = {
                    {"/root/a", "1"}, {"/root/b/c", "2.3"}, {"/root/b/d", "4"}, {"/root/e", "5"}};
                auto importer = MapImporter(values);
                std::unique_ptr<StructureValue> value;
                CHECK_NOTHROW(value = build(root, importer));
                CHECK(getValue(value) == "{1, {2.300000, 4}, 5}");
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

                ss << "1 2.3 4 5";
                CustomImporter importer(ss);
                std::unique_ptr<StructureValue> value;
                CHECK_NOTHROW(value = build(root, importer));
                CHECK(getValue(value) == "{1, {2.300000, 4}, 5}");
                CHECK(importer.onEnterBlockCount == 2);
                CHECK(importer.onExitBlockCount == 2);
            }
        }

        GIVEN ("Mixed literal and stream importers") {
            THEN ("Creating a value from correct input should work") {
                std::stringstream ss("2.3 4");
                StreamImporter<> importer(ss);
                std::unique_ptr<StructureValue> value;
                CHECK_NOTHROW(value = root.with({"1", importer, "5"}));
                CHECK(getValue(value) == "{1, {2.300000, 4}, 5}");
            }
        }
    }
}
