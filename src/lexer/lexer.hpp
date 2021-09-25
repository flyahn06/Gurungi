#include "../types/gtypes.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <regex>
#include "../utils/errors.hpp"

void initLexAnalysis(const std::string& filename);
Token analyze();
void resetPointer();
void lexAnalysisDebug();
int getCurrentLineNumber();