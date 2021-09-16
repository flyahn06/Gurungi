#include <string>

enum TokenKind {
	RBRK_1='(', RBRK_2=')', BRAC_1 = '{', BRAC_2 = '}', SBRK_1 = '[', SBRK_2 = ']',
	
	PLUS='+', MINUS='-', STAR='*', SLASH='/',   // 연산자 종류
	LESS, LESSEQ, GREAT, GREATEQ, EQUAL, NOTEQUAL,
	
	ASSIGN, COMMA, DOT, 
	
	IF, ELSE, ELIF, END,
	
	DIGIT, LETTER, INTVALUE, DOUBLEVALUE, // 어휘 분석 시 숫자와 문자로 나누어 분석하기 때문에 필요합니다.
	
	IDENTIFIER, NUMBER, STRING, OTHERS  // 어휘 분석 후 토큰에 부여되는 타입
};

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