#pragma once

#include <iosfwd>
#include <map>
#include <cstdint>
#include <tuple>

namespace archiver {
    class HuffmanArchiver {
    public:
        std::tuple<uint32_t, uint32_t, uint32_t> archive(std::istream &is, std::ostream &os) const;
        std::tuple<uint32_t, uint32_t, uint32_t> unarchive(std::istream &is, std::ostream &os) const;

    private:
        static std::map<uint8_t, uint32_t> getData(std::string const &message);
    };
}

