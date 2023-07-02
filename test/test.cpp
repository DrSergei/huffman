#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Tree.h"
#include "Writer.h"
#include "Reader.h"
#include "Application.h"

TEST_CASE("Huffman tree") {
    std::map<uint8_t, uint32_t> data;
    SUBCASE("Normal text"){
        data.insert({'a', 1});
        data.insert({'b', 2});
        data.insert({'c', 3});
        data.insert({'d', 4});
        tree::Tree tree(data);
        auto buffer = tree.getCodes();
        CHECK(buffer['a'].size() == 3);
        CHECK(buffer['b'].size() == 3);
        CHECK(buffer['c'].size() == 2);
        CHECK(buffer['d'].size() == 1);
    }
    SUBCASE("Empty text") {
        tree::Tree tree(data);
        auto buffer = tree.getCodes();
        CHECK(buffer.size() == 0);
    }
    SUBCASE("One symbol text") {
        data.insert({'a', 1});
        tree::Tree tree(data);
        auto buffer = tree.getCodes();
        CHECK(buffer.size() == 1);
        CHECK(buffer['a'].size() == 0);
    }
}

TEST_CASE("IO") {
    bool input_bool = true;
    bool output_bool = false;
    uint8_t input8_t = 123;
    uint8_t output8_t = 0;
    uint32_t input32_t = 123456;
    uint32_t output32_t = 0;
    {
        std::ofstream buffer("buffer");
        io::Writer writer(buffer);
        writer << input8_t;
        writer << input32_t;
        writer << input_bool;
        writer.flush();
        CHECK(writer.counter == sizeof(uint32_t) + 2 * sizeof(uint8_t));
    }
    {
        std::ifstream buffer("buffer");
        io::Reader reader(buffer);
        reader >> output8_t;
        reader >> output32_t;
        reader >> output_bool;
        CHECK(reader.counter == sizeof(uint32_t) + 2 * sizeof(uint8_t));
    }
    CHECK(input8_t == output8_t);
    CHECK(input32_t == input32_t);
    CHECK(input_bool == output_bool);
}

TEST_CASE("Argument parser") {
    SUBCASE("Archive"){
        char const* arguments[5];
        arguments[0] = "-c";
        arguments[1] = "-f";
        arguments[2] = "in";
        arguments[3] = "-o";
        arguments[4] = "out";
        auto buffer = app::parse(5, const_cast<char**>(arguments));
        CHECK(buffer.type == app::Argument::Type::ARCHIVE);
        CHECK(buffer.input == "in");
        CHECK(buffer.output == "out");
    }
    SUBCASE("Unarchive"){
        char const* arguments[5];
        arguments[0] = "-f";
        arguments[1] = "in";
        arguments[2] = "-u";
        arguments[3] = "-o";
        arguments[4] = "out";
        auto buffer = app::parse(5, const_cast<char**>(arguments));
        CHECK(buffer.type == app::Argument::Type::UNARCHIVE);
        CHECK(buffer.input == "in");
        CHECK(buffer.output == "out");
    }
    SUBCASE("Invalid") {
        char const* arguments[5];
        arguments[0] = "-f";
        arguments[1] = "in";
        arguments[2] = "-error";
        arguments[3] = "-o";
        arguments[4] = "out";
        auto buffer = app::parse(5, const_cast<char**>(arguments));
        CHECK(buffer.type == app::Argument::Type::INVALID);
    }
    SUBCASE("Few argument") {
        char const* arguments[3];
        arguments[0] = "-c";
        arguments[1] = "-f";
        arguments[2] = "in";
        auto buffer = app::parse(3, const_cast<char**>(arguments));
        CHECK(buffer.type == app::Argument::Type::INVALID);
    }
}