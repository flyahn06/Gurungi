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

void convert_blockSet();

// 주석작성 refactoring

bool isProcessingFunction = false;
int localAddress = 0;

void convert();
void printAllTables();



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

void expect(TokenKind k, TokenKind r) {
    cout << "k: " << k << "\t\tr: " << r << endl;
    if (k != r) error_exit("오류: 토큰의 기댓값이 일치하지 않습니다.");
}

void setCode() {}

void declareVariable() {
    token = analyze();
    if (token.kind != IDENTIFIER) {
        error_exit("\"변수\" 키위드 뒤에는 반드시 변수 이름이 와야 합니다.");
    }

    localVariables.push_back(token.intValue);


    currentSymTbl.kind = variableID;
    currentSymTbl.name = token.text;
    currentSymTbl.address = localVariables.size();

    globalSymbols.push_back(currentSymTbl);
    cout << "변수 선언 성공: " << token.text << endl;
}

// 함수 정의 처리
void declareFunction() {
    string function_name;

    token = analyze(); // 함수 키워드 스킵, 공백 스킵, 함수 이름 가져오기
    expect(IDENTIFIER, token.kind);

    function_name = token.text;

    token = analyze(); // 괄호 시작
    expect(RBRK_1, token.kind);
    do {
        token = analyze();
        cout << token.text << endl;
        convert();
    } while (token.kind != RBRK_2 || token.kind != OTHERS || token.kind != EOF_TOKEN); // 파라미터 끝
    expect(RBRK_2, token.kind);
    // 파라미터 처리 끝
    cout << "파라미터 처리 끝";

    token = analyze();
    expect(BRAC_1, token.kind);
    // TODO: 함수 몸체 처리 구현 -- 실행하면 unexpected token 오류가 발생하니다.
    return;
    expect(BRAC_2, token.kind);
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

    if (token.kind != BRAC_1) {
        error_exit("블럭이 시작되지 않았습니다." + token.text);
    }

    while(token.kind != ELIF && token.kind != ELSE && token.kind != BRAC_2 && token.kind != EOF_TOKEN) {
        convert_block();
    }

    if (token.kind == EOF_TOKEN) {
        error_exit("블럭의 끝이 정의되지 않았습니다.");
    }

    cout << "CVBLKSET_END" << endl;
}

void setCodeEnd() {}

void callFunction(int location) {
    cout << "함수 실행!" << location << endl;
}

// 주어진 토큰을 내부 코드로 변환합니다.
// 블럭을 만나면 convert_blockSet()을 실행합니다.
// convert_blockSet() <-> convert() 재귀적 실행
void convert() {
    int location;

    cout << endl << ">>> convert 호출됨: " << token.text << " <<<" << endl; 
    currentSymTbl.clear();

    if (token.kind == OTHERS) {
        intercode.push_back("\n");
        cout << "토큰 스킵" << endl;
        return;
    }

    switch(token.kind) {
        case VAR: declareVariable(); break;
        case FUNC:declareFunction(); break;
        case IDENTIFIER:
            if ((location = searchName(token.text, 'F')) != -1) {
                // 함수 호출인 경우(좀 꼬임)
                // = 만약 모든 테이블에서 변수인 이름이 없는 경우
                cout << "Callfunction" << endl;
                callFunction(location);
            }

            if ((location = searchName(token.text, 'V')) != -1) {
                // 역
                cout << "변수 찾음@" << location << endl;
                intercode.push_back("GVar");
                intercode.push_back(to_string(location));
                
            }
            break;
            // 나머지 경우에 관한 처리
        // case WHILE: case FOR:
        case IF:
            convert_blockSet();
            while (token.kind = ELIF) convert_blockSet();
            while (token.kind == ELSE) convert_blockSet();
            setCodeEnd();
            break;
            case PLUS: case MINUS: case STAR: case SLASH: case LESS: case LESSEQ: case GREAT: case GREATEQ:
            case EQUAL: case NOTEQUAL: case ASSIGN: case COMMA: case DOT: case QUOTE: case RBRK_1: case RBRK_2:
                    intercode.push_back("[" + to_string(token.kind) + "]");
                break;
        case NUMBER:
            intercode.push_back("[" + to_string(NUMBER) + "]");
            intercode.push_back(to_string(token.intValue));
            break;
        case STRING:
            intercode.push_back("[" + to_string(STRING) + "]");
            intercode.push_back(token.text);
            break;
        case PRINT:
            intercode.push_back("[" + to_string(PRINT) + "]");
            break;
        default:
            cout << RED << "! 마땅한 동작 하지 않음" << RESET << endl;
    }
    //printAllTables();
}

void printAllTables() {
    
    cout << "globalSymbols >>>" << endl;
    for (auto & globalSymbol : globalSymbols) {
        cout << globalSymbol.name << "(" << globalSymbol.kind << ")" << endl;
    }
    cout << "<<<" << endl;

    cout << "localSymbols >>>" << endl;
    for (auto & localSymbol : localSymbols) {
        cout << localSymbol.name << "(" << localSymbol.kind << ")" << endl;;
    }
    cout << "<<<" << endl;

    cout << "globalVariables >>>" << endl;
    for (int globalVariable : globalVariables) {
        cout << globalVariable << " ";
    }
    cout << "<<<" << endl;

    cout << "localVariables >>>" << endl;
    for (int localVariable : localVariables) {
        cout << localVariable << " ";
    }
    cout << "<<<" << endl;

    
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

    for (auto & globalSymbol : globalSymbols) {
        globalSymbol.printElements();
    }

    bool isLiteral = false;
    int tmp;
    for (auto & code : intercode) {
        cout << " ";

        if(isLiteral) {
            isLiteral = false;
            cout << code;
        } else if(code[0] == '[' && code[code.length()-1] == ']') {
            try {

                tmp = stoi(code.substr(1, code.length() - 2)) -1;
            } catch (const exception &e) {
                cout << code;
                continue;
            }
            if (tmp == NUMBER || tmp == STRING) {
                isLiteral = true;
            }
            cout << "[" << TokenKindMap[tmp] << "]";
        } else if (code == "GVar"){
            cout << "[GVar]";
        } else {
            cout << code;
        }
    }
}