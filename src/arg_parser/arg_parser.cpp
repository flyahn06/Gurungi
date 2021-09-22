#include "arg_parser.hpp"

using namespace std;

map<string, string> parseArguments (int argc, char* args[]) {
    map<string, string> result;

    for (int i; i < argc; i++) {
        cout << args[i] << endl;
    }

    return result;
}