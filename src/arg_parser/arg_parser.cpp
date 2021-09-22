#include "arg_parser.hpp"
#include <iostream>

using namespace std;

map<string, string> parseArguments (vector<char*> args) {
    map<string, string> result;

    for (int i; i < args.size(); i++) {
        cout << args[i] << endl;
    }

    return result;
}