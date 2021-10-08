#include "parser.hpp"

using namespace std;

vector<int> globalVariables;
vector<int> localVariables;
vector<SymbolTable> globalSymbols;
vector<SymbolTable> localSymbols;
vector<SymbolTable> blockSymbols;
GMemory DMemory;
vector<string> intercode;
SymbolTable currentSymTbl;

// 주석작성 refactoring

bool isProcessingFunction = false;
int localAddress = 0;



// 함수의 이름을 등록하는 함수입니다. 키워드 "함수" 뒤에 함수 이름이 나오지 않으면 오류를 발생시킵니다.
void set_name() {
	if (token.kind != IDENTIFIER) {
		error_exit(FuncDeclareError, "'함수' 키워드 뒤에는 함수 이름이 필요합니다.\n함수 " + token.text + CYAN + "\n     ^^^^" + RESET);
	}
    currentSymTbl.clear();
    currentSymTbl.name = token.text;
    token = analyze();
}


// 파싱 중 글로벌 영역에서의 선언인지 로컬 영역에서의 선언인지를 판단하기 위한 함수입니다.
bool is_processing_localscope() {
    return isProcessingFunction;
}

// 식별자의 이름이 로컬 영역에서 사용하는지 글로벌 영역에서 사용하는지를 판단하는 함수입니다.
bool is_localname(const string& name, SymbolKind kind) {
    // 파라미터면 무조건 로컬영역에서 사용됩니다.
    if (kind == parameterID) return true;

    // 변수의 선언일 때
    if (kind == variableID) {
        if (is_processing_localscope()) return true;  // 블럭 처리중인 경우
        else                            return false; // 아닌 경우
    }
    return false;
}

// G/L symbols 안에서 주어진 이름을 검색합니다.
// 찾으면: -1 이외의 모든 값 (예외발생)
// 찾지 못하면: -1 (정상)
int searchName(const string& name, char mode) {
    switch(mode) {
        case 'G': {
            for (int i=0; i < (int)globalSymbols.size(); i++) {
                // cout << "testing\t" << globalSymbols[i].name << endl;
                if (globalSymbols[i].name == name) return i;
            }
            break;
        }
        case 'L': {
            for (int i=0; i < (int)localSymbols.size(); i++) {
                if (localSymbols[i].name == name) return i;
            }
            break;
        }
        case 'F': {
            int n = searchName(name, 'G');
            if (n != -1 && globalSymbols[n].kind == functionID) return n;
            break;
        }
        case 'V': {
            int n;
            if (searchName(name, 'F') != -1) error_exit("함수명과 중복되었습니다.");

            if (isProcessingFunction) {
                return searchName(name, 'L');
            }

            n = searchName(name, 'G');
            if (n == -1) {
                return searchName(name, 'L');
            } else {
                return n;
            }
        }
    }
    return -1;
}

// 메모리 안에 인수로 주어진 심볼 테이블을 저장합니다.
// 자동으로 메모리를 재할당하며, 이름 중복 검사를 수행합니다.
// 리턴값은 삽입한 심볼 테이블(메모리)의 현재 크기입니다.
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

void setCode() {}

void declareVariable() {}
void declareFunction() {}

void convert_block() {
    int location;
    token = analyze();

    switch(token.kind) {
        case IDENTIFIER:
            if ((location = searchName(token.text, 'F')) == -1) {
                // 함수 호출에 대한 처리
            }

            if ((location = searchName(token.text, 'V')) == -1) {
                // 변수 선언에 관한 처리 (explicit, implicit)
            }
            // 나머지 경우에 관한 처리
    
    }
}

void convert_blockSet() {
    int endLine = 0;
    int kind = token.kind;

    token = analyze();

    if (token.kind != BRAC_1) {
        error_exit("블럭이 시작되지 않았습니다.");
    }

    while(token.kind != ELIF && token.kind != ELSE && token.kind != BRAC_2 && token.kind != EOF_TOKEN) {
        convert_block();
    }

    if (token.kind == EOF_TOKEN) {
        error_exit("블럭의 끝이 정의되지 않았습니다.");
    }
}
void setCodeEnd() {}

void convert() {
    currentSymTbl.clear();
    switch(token.kind) {
        case VAR: declareVariable(); break;
        case FUNC: declareFunction(); break;
        // case WHILE: case FOR:
        case IF:
            convert_blockSet();
            while (token.kind = ELIF) convert_blockSet();
            while (token.kind == ELSE) convert_blockSet();
            setCodeEnd();
            break;
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
            enter(currentSymTbl, functionID);
		}
	} while (token.kind != EOF_TOKEN);

    resetPointer();
    
    while (token.kind != EOF_TOKEN) {
        convert();
    }

    for (int i=0; i < globalSymbols.size(); i++) {
        globalSymbols[i].printElements();
    }
}