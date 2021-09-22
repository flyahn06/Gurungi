#include <parser.hpp>
#include "../lexer/lexer.hpp"
#include "../types/gtypes.hpp"

using namespace std;

struct Symbol {
    
};

vector<int> globalVariables;
vector<int> localVariables;
vector<Symbol> globalSymbols;
vector<Symbol> localSymbols;

Token token;

void parseIntercode(const string& filename) {
    resetPointer();

}