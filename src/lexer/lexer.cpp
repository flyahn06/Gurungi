#include "lexer.hpp"


using namespace std;

// // 토큰 원형
// struct Token {
// 	TokenKind kind;
// 	std::string text;
// 	int intValue;
// 	double doubleValue;
	
// 	Token() {
// 		kind = OTHERS;
// 		text = "";
// 		intValue = 0;
// 	}
// 	Token(TokenKind k, const std::string& s, int d) {
// 		kind = k;
// 		text = s;
// 		intValue = d;
// 	}
	
// };

// // 키워드 정의
// struct KeyWord {
// 	std::string keywordName;
// 	TokenKind keywordKind;
// };

// // 키워드 목록
// // 키워드 추가 시 TokenKind, keyWordTable에 모두 추가해야 바르게 동작함
// KeyWord keyWordTable[] = {
// 	{"만약", IF}, {"아니라면", ELSE},
// 	{"끝", END}, {"출력", PRINT},
// 	{"(", RBRK_1}, {")", RBRK_2},
// 	{"{", BRAC_1}, {"}", BRAC_2},
// 	{"[", SBRK_1}, {"]", SBRK_2},
// 	{"+", PLUS}, {"-", MINUS},
// 	{"*", STAR}, {"/", SLASH},
// 	{"=", ASSIGN}, {",", COMMA},
// 	{"==", EQUAL}, {"/=", NOTEQUAL},
// 	{"<", LESS}, {">", GREAT},
// 	{"<=", LESSEQ}, {">=", GREATEQ},
// 	{"\"", QUOTE}, {"함수", FUNC}
// };

string source;
// 현재 소스코드 문자를 가리키는 pointer값입니다.
long unsigned int pointer = 0;

// -- 어휘 분석을 위한 정규식 --
regex regAscii("[a-zA-Z0-9!?@#$%^&*():;+-=~{}<>\\_\\[\\]\\|\\\"\'\\,\\.\\/\\`\\₩]");
regex regDigit("[0-9]");
regex regOper("[\\+\\-\\*\\/<>=]");
regex regLetter("[a-zA-Z가-힣]");

// 공백 문자인지 확인
bool checkIsSpace(const string& test) {
    return test == " " || test == "\t" || test == "\n" || test == "\v" || test == "\f" || test == "\r";
}

void resetPointer() {
    pointer = 0;
}

// 전체 소스에서 다음 문자를 가져옵니다.
// return: character(string)
// 한글일 시 자동으로 3바이트, 다른 문자일 시 1바이트를 읽어 리턴합니다.

string getNextChar() {
    // 한글인지 아닌지를 판단하는 값입니다.
    // false인 경우: pointer값 1증가
    // true인 경우: pointer값 3증가 (한글이 3바이트이므로)
    static bool isHangul = false;
    

    // cout << "PTR:" << pointer << endl;
    if (isHangul) {pointer++; pointer++; isHangul = false;}
    pointer++;

    // 포인터가 소스코드의 마지막 문자를 가리키고 있는 경우입니다.
    if (pointer > source.length()) {
        return "SOURCE_EOF";
    }
    
    // 먼저 한 글자만을 읽어옵니다.
    string temp = source.substr(pointer-1, 1);

    if (regex_match(temp, regAscii) || checkIsSpace(temp)) {
        // 만약 아스키코드나 공백 중 하나라면 (한글이 아니라면) 바로 리턴합니다.
        return temp;
    }
    
    else {
        // 한글이라면 3바이트를 읽어 불러옵니다.
        isHangul = true;
        return source.substr(pointer-1, 3);
    }
}

// 연산자인지 판단합니다.
bool isOperator(const string& test) {
    return regex_match(test, regOper);
}

// 분석해 토큰을 리턴하는 함수입니다.
Token analyze() {
    static string ch0 = "";  // 이전 문자
    string ch;
    string text = "";

    // 판단할 이전 문자가 있는 경우, ch값을 새로 불러오지 않고
    // 이전에 저장되어 있던 값을 사용합니다.
    if (ch0 != "") {ch = ch0; ch0 = "";}
    else            ch = getNextChar();
    while (checkIsSpace(ch)) ch = getNextChar();
    
    // EOF를 받은 경우 EOF토큰을 반환합니다.
    if (ch == "SOURCE_EOF") {
        return Token(EOF_TOKEN, "SOURCE_EOF", 0);
    }
    
    // 숫자가 있는 경우 숫자를 모두 불러와 토큰을 추출합니다.
    if (regex_match(ch, regDigit)) {
        do {
            text += ch;
            ch = getNextChar();
        } while(regex_match(ch, regDigit) && ch != "SOURCE_EOF");

        ch0 = ch;

        return Token(NUMBER, "", stoi(text));

    } 
    
    // 문자열 (") 가 시작된 경우 끝까지 읽어옵니다.
    else if(ch == "\"") {
        // " 문자 스킵
        ch = getNextChar();
        do {
            text += ch;
            ch = getNextChar();
            if (ch == "SOURCE_EOF") error_exit("어휘분석 오류: \"가 닫히지 않았습니다.");
        } while (ch != "\"");

        return Token(STRING, text, 0);

    } 
    
    else {
        // 문자가 연산자인 경우
        if (isOperator(ch)) {
            // 2개 이상 결합한 연산자 (>=, <= 등) 인 경우를 대비한 코드입니다.
            ch0 = ch;
            ch = getNextChar();
            
            if (isOperator(ch0 + ch)) {
                text = ch0 + ch;
                ch0 = "";
            } else {
                text = ch0;
                ch0 = ch;
            }
        }
        
        else {
            do {
                text += ch;
                ch = getNextChar();
                if (isOperator(ch)) {
                    ch0 = ch;
                    break;
                }
            } while (!checkIsSpace(ch) && !(ch == "SOURCE_EOF"));

        }

        
        for (long unsigned int i; i < sizeof(keyWordTable)/sizeof(keyWordTable[0]); i++) {
            if (text == keyWordTable[i].keywordName) {
                // cout << text << "\t" << keyWordTable[i].keywordName << endl;
                return Token(keyWordTable[i].keywordKind, keyWordTable[i].keywordName, 0);
            }
        }

        return Token(IDENTIFIER, text, 0);
    }
    
}

// 실제로 호출되는 함수입니다.
void lexAnalysis(const string& _source){
    source = _source;
    cout << "받은 소스: " << endl
    << source << endl
    << "에 대한 어휘 분석을 시작합니다." << endl;

    cout << "Kind\tText\t\tIntVal" << endl;

    do {
        token = analyze();
        cout << token.kind << "\t" << token.text << "\t\t" << token.intValue << endl;
    } while (token.kind != EOF_TOKEN);
    
}