#include "HuffmanArchiver.h"
#include "Tree.h"
#include "Reader.h"
#include "Writer.h"

#include <numeric>
#include <iostream>
#include <sstream>

namespace archiver {
    using tree::Tree;
    using io::Writer;
    using io::Reader;

    std::tuple<uint32_t, uint32_t, uint32_t> HuffmanArchiver::archive(std::istream &is, std::ostream &os) const {
        try {
            std::stringstream buffer;
            buffer << is.rdbuf();
            std::string message = buffer.str();
            std::stringstream().swap(buffer);
            auto data = getData(message);
            Tree tree(data);
            auto codes = tree.getCodes();
            Writer writer(os);
            writer << static_cast<uint32_t>(data.size());
            if (data.empty()) { // empty text
                buffer << 0 << "\n";
                buffer << 0 << "\n";
                buffer << sizeof(uint32_t) << "\n";
                return {0, 0, sizeof(uint32_t)};
            }
            for (auto [ch, number]: data)
                writer << ch << number;

            // normal text
            for (auto ch: message)
                writer << codes[ch];
            writer.flush();

            std::size_t added_size = data.size() * (sizeof(uint32_t) + sizeof(uint8_t)) + sizeof(uint32_t);
            return {message.size(), writer.counter - added_size, added_size};
        } catch (std::exception& e) {
            std::cerr << e.what();
            return {0, 0, 0};
        }
    }

    std::tuple<uint32_t, uint32_t, uint32_t> HuffmanArchiver::unarchive(std::istream &is, std::ostream &os) const {
        try {
            std::stringstream buffer;
            Reader reader(is);
            std::map<uint8_t, uint32_t> data;
            std::uint32_t size;
            reader >> size;
            if (size == 0) {// empty text
                buffer << 0 << "\n";
                buffer << 0 << "\n";
                buffer << reader.counter << "\n";
                return {0, 0, reader.counter};
            }
            for (uint32_t i = 0; i < size; i++) {
                uint8_t ch;
                uint32_t number;
                reader >> ch >> number;
                data.insert({ch, number});
            }
            if (size == 1) { // one symbol text
                for (std::uint32_t i = 0; i < data.begin()->second; i++)
                    os << static_cast<char>((data.begin()->first));
                buffer << 0 << "\n";
                buffer << data.begin()->second << "\n";
                buffer << reader.counter << "\n";
                return {0, data.begin()->second, reader.counter};
            }

            // normal text
            Tree tree(data);
            uint32_t uncompressed_size = std::accumulate(data.begin(), data.end(), 0,[](uint32_t acc, std::pair<uint8_t, uint32_t> const &data) {return acc + data.second;});
            for (uint32_t i = 0; i < uncompressed_size; i++) {
                bool bit;
                do {
                    reader >> bit;
                } while (tree.move(bit));
                os << tree.getChar();
                tree.moveToRoot();
            }

            std::size_t added_size = data.size() * (sizeof(uint32_t) + sizeof(uint8_t)) + sizeof(uint32_t);
            return {reader.counter - added_size, uncompressed_size, added_size};
        } catch (std::exception& e) {
            std::cerr << e.what();
            return {0, 0, 0};
        }
    }

    std::map<uint8_t, uint32_t> HuffmanArchiver::getData(const std::string &message) {
        std::map<uint8_t, uint32_t> data;
        for (auto i: message)
            data[static_cast<uint8_t>(i)]++;
        return data;
    }
}