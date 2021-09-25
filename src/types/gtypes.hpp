#include <string>
#include <iostream>

#define KEYCOUNT 26
#ifndef _GTYPES_HPP_
#define _GTYPES_HPP_

enum TokenKind {
	RBRK_1=1, RBRK_2, BRAC_1, BRAC_2, SBRK_1, SBRK_2,
	
	PLUS, MINUS, STAR, SLASH,   // 연산자 종류
	LESS, LESSEQ, GREAT, GREATEQ, EQUAL, NOTEQUAL,
	
	ASSIGN, COMMA, DOT, QUOTE,
	
	PRINT,
	IF, ELSE, ELIF, WHILE, FOR,
	FUNC, VAR,
	END,
	
	DIGIT, LETTER, INTVALUE, DOUBLEVALUE, // 어휘 분석 시 숫자와 문자로 나누어 분석하기 때문에 필요합니다.
	
	IDENTIFIER, NUMBER, STRING, OTHERS,  // 어휘 분석 후 토큰에 부여되는 타입
	EOF_TOKEN

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
	Token(TokenKind k, const std::string& s, int d) {
		kind = k;
		text = s;
		intValue = d;
	}
	
};

struct KeyWord {
	std::string keywordName;
	TokenKind keywordKind;
};

enum SymbolKind {
	noID, variableID, functionID, parameterID
};

enum dataType {
	noType, intType // 추후 추가 예정
};

struct SymbolTable {
	std::string      name;        // 명시한 이름
	SymbolKind       kind;        // 함수/변수/매개변수
	dataType         type;        // 가지고 있는 데이터의 타입
	int              arrayLength; // 배열일 시 길이 기입 (단순변수는 0)
	short            args;        // 함수일 시 인자 개수 기입
	int              address;     // 변수/함수의 주소 (global | local index)
	int              frame;       // 함수일 시 프레임 크기 기입
	
	SymbolTable() {
		clear();
	}
	
	void clear();
	void printElements();
};

#endif
extern KeyWord keyWordTable[KEYCOUNT];
extern Token token;