#pragma once

#include <fstream>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace io {
    class Reader {
    public:
        explicit Reader(std::istream &is);
        Reader(Reader const&) = delete;
        Reader& operator=(Reader const&) = delete;
        Reader(Reader&&) = delete;
        Reader& operator=(Reader&&) = delete;
        ~Reader() = default;

        template<typename T>
        friend std::enable_if_t<std::is_unsigned_v<T>, Reader&> operator>>(Reader &reader, T& number);

        explicit operator bool() const;

        uint32_t counter;
    private:
        std::istream &is;
        uint8_t buffer;
        uint8_t size;
    };

    template<typename T>
    std::enable_if_t<std::is_unsigned_v<T>, Reader&> operator>>(Reader &reader, T& number) {
        T result = 0;
        auto bits = static_cast<size_t>(std::numeric_limits<T>::digits);
        for (size_t i = 0; i < bits; i++) {
            bool bit;
            reader >> bit;
            if (bit)
                result |= (static_cast<T>(1) << (bits - i - 1));
        }
        number = result;
        return reader;
    }

    template<>
    inline Reader& operator>><bool>(Reader &reader, bool &bit) {
        if (reader.size == 0) {
            reader.buffer = reader.is.get();
            reader.size = std::numeric_limits<uint8_t>::digits;
            reader.counter++;
        }
        bit = reader.buffer & static_cast<uint8_t>(1);
        reader.size--;
        reader.buffer >>= 1;
        return reader;
    }
}