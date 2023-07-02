#include "Reader.h"

namespace io {
    Reader::Reader(std::istream &is) : counter(0), is(is), buffer(0), size(0) {
        is.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    }

    Reader::operator bool() const {
        return static_cast<bool>(is);
    }
}