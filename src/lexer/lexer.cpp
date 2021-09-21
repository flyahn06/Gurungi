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
	Token(TokenKind k, const std::string& s, int d) {
		kind = k;
		text = s;
		intValue = d;
	}
	
};

struct KeyWord {
	string keywordName;
	TokenKind keywordKind;
};

KeyWord keyWordTable[] = {
	{"만약", IF}, {"아니라면", ELSE},
	{"끝", END}, {"출력", PRINT},
	{"(", RBRK_1}, {")", RBRK_2},
	{"{", BRAC_1}, {"}", BRAC_2},
	{"[", SBRK_1}, {"]", SBRK_2},
	{"+", PLUS}, {"-", MINUS},
	{"*", STAR}, {"/", SLASH},
	{"=", ASSIGN}, {",", COMMA},
	{"==", EQUAL}, {"/=", NOTEQUAL},
	{"<", LESS}, {">", GREAT},
	{"<=", LESSEQ}, {">=", GREATEQ},
	{"\"", QUOTE}
};

Token token;
int pointer;
string source;
regex regAscii("[a-zA-Z0-9!?@#$%^&*():;+-=~{}<>\\_\\[\\]\\|\\\"\'\\,\\.\\/\\`\\₩]");
regex regDigit("[0-9]");
regex regOper("[+\\-*\\/<>=]");
regex regLetter("[a-zA-Z가-힣]");
bool isHangul = false;

bool checkIsSpace(const string& test) {
    return test == " " || test == "\t" || test == "\n" || test == "\v" || test == "\f" || test == "\r";
}

string getNextChar() {
    // cout << "PTR:" << pointer << endl;
    if (isHangul) {pointer++; pointer++; isHangul = false;}
    pointer++;

    if (pointer > source.length()) {
        return "SOURCE_EOF";
    }
    
    string temp = source.substr(pointer-1, 1);
    if (regex_match(temp, regAscii) || checkIsSpace(temp)) {
        return temp;
    } else {
        isHangul = true;
        return source.substr(pointer-1, 3);
    }
}

bool isOperator(const string& test) {
    return regex_match(test, regOper);
}

Token analyze() {
    static string ch0 = "";
    string ch;
    string text = "";

    if (ch0 != "") {ch = ch0; ch0 = "";}
    else            ch = getNextChar();
    while (checkIsSpace(ch)) ch = getNextChar();
    // if (ch0 == "") {
    //     // cout << "Getting new char" << endl;
    //     ch = getNextChar();
        
    // } 
    // else {
    //     // cout << "ch0 found" << endl;
    //     ch = ch0;
    //     ch0 = "";
    // }
    
    // cout << "ch: "<< ch << endl;

    // cout << ch << endl;

    if (ch == "SOURCE_EOF") {
        return Token(EOF_TOKEN, "SOURCE_EOF", 0);
    }
    

    if (regex_match(ch, regDigit)) {
        do {
            text += ch;
            ch = getNextChar();
        } while(regex_match(ch, regDigit) && ch != "SOURCE_EOF");

        ch0 = ch;

        return Token(NUMBER, "", stoi(text));

    } 
    
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
        if (isOperator(ch)) {
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

        
        for (int i; i < sizeof(keyWordTable)/sizeof(keyWordTable[0]); i++) {
            if (text == keyWordTable[i].keywordName) {
                // cout << text << "\t" << keyWordTable[i].keywordName << endl;
                return Token(keyWordTable[i].keywordKind, keyWordTable[i].keywordName, 0);
            }
        }

        return Token(IDENTIFIER, text, 0);
    }
    
}

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