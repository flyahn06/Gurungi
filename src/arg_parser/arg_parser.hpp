#ifndef _ARG_PARSER_HPP_
#define _ARG_PARSER_HPP_

#include "../Gurungi.hpp"
#include <map>
#include <string>

std::map<std::string, std::string> parseArguments(int argc, char* args[]);

#endif