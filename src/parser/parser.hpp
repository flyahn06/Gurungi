#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <vector>
#include <fstream>
#include "../Gurungi.hpp"

extern std::vector<int> globalVariables;
extern std::vector<int> localVariables;
extern std::vector<SymbolTable> globalSymbols;
extern std::vector<SymbolTable> localSymbols;
extern std::vector<SymbolTable> blockSymbols;
extern std::vector<std::string> intercode;

void parseIntercode();

#endif