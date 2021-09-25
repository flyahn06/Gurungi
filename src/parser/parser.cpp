#include "parser.hpp"

using namespace std;

vector<int> globalVariables;
vector<int> localVariables;
vector<SymbolTable> globalSymbols;
vector<SymbolTable> localSymbols;
GMemory DMemory;
vector<string> intercode;
SymbolTable currentSymTbl;

bool isProcessingFunction = false;
int localAddress = 0;

void set_name() {
	if (token.kind != IDENTIFIER) {
		error_exit(FuncDeclareError, "'함수' 키워드 뒤에는 함수 이름이 필요합니다.\n함수 " + token.text + CYAN + "\n     ^^^^" + RESET);
	}
    currentSymTbl.clear();
    currentSymTbl.name = token.text;
    token = analyze();
}

bool is_processing_localscope() {
    return isProcessingFunction;
}

bool is_localname(const string& name, SymbolKind kind) {
    if (kind == parameterID) return true;
    if (kind == variableID) {
        if (is_processing_localscope()) return true;
        else                            return false;
    }
    return false;
}

// G/L symbols 안에서 주어진 이름을 검색합니다.
// 찾으면: -1 이외의 모든 값 (예외발생)
// 찾지 못하면: -1 (정상)
int searchName(const string& name, char scope) {
    // TODO: GlobalST/LocalST 에서 이름 검색 구현
    return -1;
}

int enter(SymbolTable& table, SymbolKind kind) {
    int n, memorySize;
    bool isLocal = is_localname(table.name, kind);

    memorySize = table.arrayLength;
    if (memorySize == 0) memorySize = 1;
    table.kind = kind;
    n = -1;

    if (kind == functionID)        n = searchName(table.name, 'G');
    else if (kind == parameterID)  n = searchName(table.name, 'L');
    if (n != -1) error_exit("이름 중복 오류", "이름이 중복되었습니다: " + table.name);

    if (kind == functionID) table.address = getCurrentLineNumber();
    else {
        if (isLocal) {
            table.address = localAddress + memorySize;
        } else {
            table.address = DMemory.size();
            DMemory.resize(DMemory.size() + memorySize);
        }
    }

    if (isLocal) {
        n = localSymbols.size();
        localSymbols.push_back(table);
    } else {
        n = globalSymbols.size();
        globalSymbols.push_back(table);
    }

    return n;
    
}

void parseIntercode() {
    resetPointer();
	do {
		token = analyze();
		// cout << (token.kind == FUNC) << endl;
		if (token.kind == FUNC) {
			token = analyze();
			set_name();
            enter(currentSymTbl, functionID);
		}
	} while (token.kind != EOF_TOKEN);

    for (int i=0; i < globalSymbols.size(); i++) {
        globalSymbols[i].printElements();
    }
}