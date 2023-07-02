#include "Application.h"

#include <iostream>
#include <fstream>

namespace app {
    using archiver::HuffmanArchiver;

    void run(Argument const &argument) {
        auto archiver = HuffmanArchiver{};
        switch (argument.type) {
            case Argument::Type::INVALID: {
                std::cerr << "Invalid arguments";
                break;
            }
            case Argument::Type::ARCHIVE: {
                std::ifstream is(argument.input, std::ifstream::in);
                std::ofstream os(argument.output, std::ofstream::out);
                auto buffer = archiver.archive(is, os);
                if (buffer == std::make_tuple(0, 0, 0)) {
                    std::cout << "Error";
                    break;
                }
                std::cout << get<0>(buffer) << "\n";
                std::cout << get<1>(buffer) << "\n";
                std::cout << get<2>(buffer) << "\n";
                break;
            }
            case Argument::Type::UNARCHIVE: {
                std::ifstream is(argument.input, std::istream::in);
                std::ofstream os(argument.output, std::ofstream::out);
                auto buffer = archiver.unarchive(is, os);
                if (buffer == std::make_tuple(0, 0, 0)) {
                    std::cout << "Error";
                    break;
                }
                std::cout << get<0>(buffer) << "\n";
                std::cout << get<1>(buffer) << "\n";
                std::cout << get<2>(buffer) << "\n";
                break;
            }
        }
    }

    Argument parse(int argc, char **argv) {
        Argument argument{Argument::Type::INVALID, "", ""};
        for (int i = 0; i < argc; i++) {
            std::string_view buffer(argv[i]);
            if (buffer == "-c" && argument.type == Argument::Type::INVALID)
                argument.type = Argument::Type::ARCHIVE;
            if (buffer == "-u" && argument.type == Argument::Type::INVALID)
                argument.type = Argument::Type::UNARCHIVE;
            if (buffer ==  "-f" || buffer == "--file") {
                if (i < argc - 1)
                    argument.input = std::string_view(argv[++i]);
            }
            if (buffer == "-o" || buffer == "--output") {
                if (i < argc - 1)
                    argument.output = std::string_view(argv[++i]);
            }
        }
        if (argument.input.empty() || argument.output.empty())
            argument.type = Argument::Type::INVALID;
        return argument;
    }
}