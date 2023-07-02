#pragma once

#include <iosfwd>
#include <vector>
#include <cstdint>
#include <limits>

namespace io {
    class Writer {
    public:
        explicit Writer(std::ostream &os);
        Writer(Writer const&) = delete;
        Writer& operator=(Writer const&) = delete;
        Writer(Writer&&) = delete;
        Writer& operator=(Writer&&) = delete;
        ~Writer();

        template<typename T>
        friend std::enable_if_t<std::is_unsigned_v<T>, Writer&> operator<<(Writer &writer, T number);

        friend Writer &operator<<(Writer &writer, std::vector<bool> const &bits);

        void flush();

        explicit operator bool() const;

        uint32_t counter;
    private:
        std::ostream &os;
        uint8_t buffer;
        uint8_t size;
        uint8_t const capacity = 8;
    };

    template<typename T>
    std::enable_if_t<std::is_unsigned_v<T>, Writer&> operator<<(Writer &writer, T number) {
        auto bits = static_cast<size_t>(std::numeric_limits<T>::digits);
        for (size_t i = 0; i < bits; i++)
            writer << static_cast<bool>((number >> (bits - i - 1)) & static_cast<T>(1));
        return writer;
    }

    template<>
    inline Writer &operator<<<bool>(Writer &writer, bool bit) {
        if (writer.size == writer.capacity)
            writer.flush();
        if (bit)
            writer.buffer |= (1 << writer.size);
        writer.size++;
        return writer;
    }
}