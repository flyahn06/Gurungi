#include "arg_parser.hpp"

std::map<std::string, std::string> parseArguments (int argc, char* args[]) {
    std::map<std::string, std::string> result;

    for (int i; i < argc; i++) {
        std::cout << args[i] << std::endl;
    }

    return result;
}