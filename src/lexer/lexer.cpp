#include "lexer.hpp"
#include "../utils/errors.hpp"
#include <regex>

using namespace std;


struct Token {
	TokenKind kind;
	std::string text;
	int intValue;
	double doubleValue;
	
	Token() {
		kind = OTHERS;
		text = "";
		intValue = 0;
	}
	Token(TokenKind k, const std::string& s, int d=0) {
		kind = k;
		text = s;
		intValue = 0;
	}
	
};

struct KeyWord {
	std::string keywordName;
	TokenKind keywordKind;
};

KeyWord keyWordTable[] = {
	{"만약", IF}, {"아니라면", ELSE},
	{"끝", END}, {"프린트", PRINT},
	{"(", RBRK_1}, {")", RBRK_2},
	{"{", BRAC_1}, {"}", BRAC_2},
	{"[", SBRK_1}, {"]", SBRK_2},
	{"+", PLUS}, {"-", MINUS},
	{"*", STAR}, {"/", SLASH},
	{"=", ASSIGN}, {",", COMMA},
	{"==", EQUAL}, {"/=", NOTEQUAL},
	{"<", LESS}, {">", GREAT},
	{"<=", LESSEQ}, {">=", GREATEQ},
	{"\"", QUOTE}, {"", END_LIST}
};

Token token;
int pointer;
string source;
regex regAscii("[a-zA-Z0-9!?@#$%^&*():;+-=~{}<>\\_\\[\\]\\|\\\"\'\\,\\.\\/\\`\\₩]");
regex regDigit("[0-9]")

bool checkIsSpace(const string& test) {
    return test == " " || test == "\t" || test == "\n" || test == "\v" || test == "\f" || test == "\r";
}

string getNextChar() {
    static bool isHangul = false;
    if (isHangul) {pointer++; pointer++; isHangul = false;}
    pointer++;

    if (pointer > source.length()) {
        return Token(EOF_TOKEN, "EOF");
    }
    
    string temp = source.substr(pointer-1, 1);
    if (regex_match(temp, regAscii) || checkIsSpace(temp)) {
        return temp;
    } else {
        isHangul = true;
        return source.substr(pointer-1, 3);
    }
}

Token analyze() {
    static string ch0 = "";
    string ch = getNextChar();
    string text = ""

    while (checkIsSpace(ch)) ch = getNextChar();
    

    if (regex_match(ch, regDigit)) {
        text += ch;
        while ()
        

    }
    
    return Token();
}

void lexAnalysis(const string& _source){
    source = _source;
    cout << "받은 소스: " << endl
    << source << endl
    << "에 대한 어휘 분석을 시작합니다." << endl;

    analyze();
    
}