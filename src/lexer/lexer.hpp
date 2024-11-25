#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include "../Gurungi.hpp"
#include <regex>

void initLexAnalysis(const std::string& filename);
Token analyze();
void resetPointer();
void lexAnalysisDebug();
int getCurrentLineNumber();

#endif