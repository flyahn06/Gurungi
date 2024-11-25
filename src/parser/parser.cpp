#include "parser.hpp"

std::vector<int> globalVariables;
std::vector<int> localVariables;
std::vector<SymbolTable> globalSymbols;
std::vector<SymbolTable> localSymbols;
std::vector<SymbolTable> blockSymbols;
GMemory DMemory;
std::vector<std::string> intercode;
SymbolTable currentSymTbl;

bool isProcessingFunction = false;
int localAddress = 0;
int currently_processing_function_addr = 0;

void convert_blockSet();
void convert();
void printAllTables();
void expect(TokenKind, TokenKind);
void expect(TokenKind, TokenKind, const std::string&);

// 함수의 이름을 등록하는 함수입니다. 키워드 "함수" 뒤에 함수 이름이 나오지 않으면 오류를 발생시킵니다.
void set_name() {
	if (token.kind != IDENTIFIER) {
		error_exit(FuncDeclareError, getCurrentLineNumber(), "'함수' 키워드 뒤에는 함수 이름이 필요합니다.\n함수 " +
		    token.text + CYAN + "\n    " + std::string(token.text.length(), '^') + RESET);
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
bool is_localname(const std::string& name, SymbolKind kind) {
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
int searchName(const std::string& name, char mode) {
    switch(mode) {
        case 'G': {
            // Global Symbol Table에서 검색 (Private)
            for (int i=0; i < (int)globalSymbols.size(); i++) {
                // cout << "testing\t" << globalSymbols[i].name << endl;
                if (globalSymbols[i].name == name) return i;
            }
            break;
        }
        case 'L': {
            // Local Symbol Table에서 검색 (Private)
            for (int i=0; i < (int)localSymbols.size(); i++) {
                if (localSymbols[i].name == name) return i;
            }
            break;
        }
        case 'F': {
            // 함수 이름 검색 (Public)
            // 함수 이름은 GST에 들어가므로 GST만을 검색함.
            int n = searchName(name, 'G');
            if (n != -1 && globalSymbols[n].kind == functionID) return n;
            break;
        }
        case 'V': {
            // 변수 이름 검색 (Public)
            int n;

            // 함수명과 중복 검사
            if (searchName(name, 'F') != -1) error_exit("함수명과 중복되었습니다.");

            // 현재 함수 처리중일 때 -> LST에서 찾음
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

// 현재 토큰을 assert합니다.
void expect(TokenKind k, TokenKind r, const std::string& errormsg) {
    if (k != r) error_exit(errormsg);
}

void expect(TokenKind k, TokenKind r) {
    if (k != r) {
        error_exit(SyntaxError, getCurrentLineNumber(), "토큰의 기댓값이 일치하지 않습니다. (Expected " + TokenKindMap[k] +
            " but got " + TokenKindMap[r] + ")");
    }
}

void expect(TokenKind k, TokenKind r, std::string &errormsg) {
    if (k != r) error_exit(LexerError,errormsg);
}

void setCode() {}

void declareVariable() {
    token = analyze();
    if (token.kind != IDENTIFIER) {
        error_exit(SyntaxError, getCurrentLineNumber(), "'변수' 키워드 뒤에는 변수 이름이 필요합니다.\n변수 " +
            token.text + CYAN + "\n    " + std::string(token.text.length(), '^') + RESET);
    }

    if (is_processing_localscope()) localVariables.push_back(token.intValue);
    else globalVariables.push_back(token.intValue);

    currentSymTbl.kind = variableID;
    currentSymTbl.name = token.text;
    currentSymTbl.address = localVariables.size();  // "push"이므로 이 변수의 위치는 현재 LVT의 크기가 됨

    if (is_processing_localscope()) localSymbols.push_back(currentSymTbl);
    else globalSymbols.push_back(currentSymTbl);
}

// 함수 정의 처리
void declareFunction() {
    isProcessingFunction = true;  // 이제부터 나오는 심볼들은 LST에 들어감

    // 시그니처 처리
    // 함수 이름 처리
    token = analyze(); // 함수 키워드 스킵, 공백 스킵 -> 이제 token은 함수 이름이어야 함
    expect(IDENTIFIER, token.kind);
    currently_processing_function_addr = searchName(token.text, 'F');

    // 파라미터 처리
    token = analyze();
    expect(RBRK_1, token.kind);

    do {
        token = analyze();

        if (token.kind != RBRK_2) {
            if (token.kind != COMMA) {
                expect(IDENTIFIER, token.kind);  // 여기는 파라미터를 처리하는 부분이므로 무조건 식별자여야 함
                convert();
            } else {
                // 컴마인 경우는 씹음
            }
        } else break;

    } while (token.kind != OTHERS && token.kind != EOF_TOKEN);

    expect(RBRK_2, token.kind);
    // 파라미터 처리 끝

    // 함수 본체 처리
    token = analyze();
    expect(BRAC_1, token.kind);

    do {
        token = analyze();
        if (token.kind != BRAC_2) convert();
        else break;
    } while (token.kind != BRAC_2);

    expect(BRAC_2, token.kind);
    isProcessingFunction = false;  // 함수 처리 끝
}


// 실제로 라인을 처리하는 함수입니다.
void convert_block() {
    int location;
    token = analyze(); // 다음 토큰을 불러옴
    convert();
}

// convert() 에서 불려집니다.
void convert_blockSet() {
    // 블럭이 끝나는 라인의 번호
    int endLine = 0;
    // 현재 토큰의 종류 (FUNC, FOR, IF, WHILE 등등)
    int kind = token.kind;

    // 다음 토큰을 불러옴
    token = analyze();
    while (token.kind != BRAC_1 && token.kind != EOF_TOKEN) convert_block();

    expect(BRAC_1, token.kind, "블럭이 시작되지 않았습니다. " + token.text);

    while(token.kind != ELIF && token.kind != ELSE && token.kind != BRAC_2 && token.kind != EOF_TOKEN) {
        convert_block();
    }

    expect(BRAC_2, token.kind, "블럭이 끝나지 않았습니다. " + token.text);
}

void setCodeEnd() {}

void callFunction(int location) {
    intercode.push_back("CallFunc");
    intercode.push_back(std::to_string(location));
}

// 주어진 토큰을 내부 코드로 변환합니다.
// 블럭을 만나면 convert_blockSet()을 실행합니다.
void convert() {
    int location;

    currentSymTbl.clear();

    if (token.kind == NEW_LINE) {
        intercode.push_back("\n");
        return;
    }

    switch(token.kind) {
        case VAR:  declareVariable(); break;
        case FUNC: declareFunction(); break;
        case IDENTIFIER:
            if ((location = searchName(token.text, 'F')) != -1) {
                // 함수 호출인 경우(좀 꼬임)
                // = 만약 모든 테이블에서 변수인 이름이 없는 경우
                callFunction(location);
            }

            else if ((location = searchName(token.text, 'V')) != -1) {
                // 여기도 없을 때
                if (is_processing_localscope()) {
                    intercode.push_back("LVar");
                    intercode.push_back(std::to_string(location));
                } else {
                    intercode.push_back("GVar");
                    intercode.push_back(std::to_string(location));
                }
            }

            else if (is_processing_localscope()) {
                // 여기는 함수 파라미터인 경우임
                globalSymbols[currently_processing_function_addr].args += 1;
            }
            break;
            // 나머지 경우에 관한 처리
        case WHILE: case FOR:
            // TODO: while, for 구현
            break;
        case IF:
            convert_blockSet();
            while (token.kind == ELIF) convert_blockSet();
            while (token.kind == ELSE) convert_blockSet();
            setCodeEnd();
            break;
            case PLUS: case MINUS: case STAR: case SLASH: case LESS: case LESSEQ: case GREAT: case GREATEQ:
            case EQUAL: case NOTEQUAL: case ASSIGN: case COMMA: case DOT: case QUOTE: case RBRK_1: case RBRK_2:
                    intercode.push_back("[" + std::to_string(token.kind) + "]");
                break;
        case NUMBER:
            intercode.push_back("[" + std::to_string(NUMBER) + "]");
            intercode.push_back(std::to_string(token.intValue));
            break;
        case STRING:
            intercode.push_back("[" + std::to_string(STRING) + "]");
            intercode.push_back(token.text);
            break;
        case PRINT:
            intercode.push_back("[" + std::to_string(PRINT) + "]");
            break;
        case EOF_TOKEN:
            std::cout << "파싱 작업이 끝났습니다." << std::endl;
            break;
    }
}

void printAllTables() {
    std::cout << "globalSymbols:" << std::endl;
    for (auto & globalSymbol : globalSymbols) {
        std::cout << "\t" << globalSymbol.name << "(" << globalSymbol.kind << ")" << std::endl;
    }

    std::cout << "localSymbols:" << std::endl;
    for (auto & localSymbol : localSymbols) {
        std::cout << "\t" << localSymbol.name << "(" << localSymbol.kind << ")" << std::endl;
    }

    std::cout << "globalVariables:" << std::endl;
    for (int globalVariable : globalVariables) {
        std::cout << "\t" << globalVariable << " ";
    }
    std::cout << std::endl;

    std::cout << "localVariables: " << std::endl;
    for (int localVariable : localVariables) {
        std::cout << "\t" << localVariable << " ";
    }
    std::cout << std::endl;
}

void parseIntercode() {
    resetPointer();
    // 소스코드에서 함수 이름만 읽어들여서 임시로 메모리에 삽입
	do {
		token = analyze();
		if (token.kind == FUNC) {
			token = analyze();
			set_name();
            enter(currentSymTbl, functionID);
		}
	} while (token.kind != EOF_TOKEN);


    // lexer 초기화
    resetPointer();
    token = analyze();

    // 모든 블럭을 내부 코드로 파싱
    while (token.kind != EOF_TOKEN) {
        convert();
        token = analyze();
    }


    // ------------------------------------------------------------

    bool isLiteral = false;
    int tmp;
    int line = 1;

    for (auto & code : intercode) {
        std::cout << " ";

        if (code == "\n") {
            std::cout << "\nln " << line << ":";
            line++;
            continue;
        }

        if(isLiteral) {
            isLiteral = false;
            std::cout << code;
        } else if(code[0] == '[' && code[code.length()-1] == ']') {
            try {
                tmp = stoi(code.substr(1, code.length() - 2));
            } catch (const std::exception &e) {
                std::cout << code;
                continue;
            }
            if (tmp == NUMBER || tmp == STRING) {
                isLiteral = true;
            }
            std::cout << "[" << TokenKindMap[tmp] << "]";
        } else if (code == "GVar") {
            std::cout << "[GVar]";
        } else if (code == "LVar") {
            std::cout << "[LVar]";
        } else if (code == "CallFunc") {
            std::cout << "[CallFunc]";
        } else {
            std::cout << code;
        }
    }
    std::cout << std::endl;
    printAllTables();
}
