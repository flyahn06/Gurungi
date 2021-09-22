#include "parser.hpp"

using namespace std;

vector<int> globalVariables;
vector<int> localVariables;
vector<SymbolTable> globalSymbols;
vector<SymbolTable> localSymbols;
vector<string> intercode;

void set_name() {
	if (token.kind != IDENTIFIER) {
		error_exit(FuncDeclareError, "'함수' 키워드 뒤에는 함수 이름이 필요합니다.\n함수 " + token.text + CYAN + "\n     ^^^^" + RESET);
		
	}
}

void parseIntercode() {
    resetPointer();
	do {
		token = analyze();
		// cout << (token.kind == FUNC) << endl;
		if (token.kind == FUNC) {
			token = analyze();
			set_name();
		}
	} while (token.kind != EOF_TOKEN);
}