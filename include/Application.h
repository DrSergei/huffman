#pragma once

#include "HuffmanArchiver.h"

#include <string>

namespace app {

    struct Argument {
        enum class Type {
            ARCHIVE,
            UNARCHIVE,
            INVALID
        };
        Type type;
        std::string input;
        std::string output;
    };

    Argument parse(int argc, char **argv);


    void run(app::Argument const &argument);
}