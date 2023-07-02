#include "Writer.h"

#include <fstream>
#include <limits>

namespace io {
    Writer::Writer(std::ostream &os) : counter(0),  os(os), buffer(0), size(0) {
        os.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    }

    Writer &operator<<(Writer &writer, const std::vector<bool> &bits) {
        for (auto bit: bits)
            writer << bit;
        return writer;
    }

    Writer::~Writer() {
        flush();
    }

    void Writer::flush() {
        if (size == 0)
            return;
        os << buffer;
        size = 0;
        buffer = 0;
        counter++;
    }

    Writer::operator bool() const {
        return (bool) os;
    }
}