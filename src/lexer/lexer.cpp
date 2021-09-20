#include "lexer.hpp"

TokenKind charType[256];

using namespace std;

void initCharType(){
	// CharType 배열을 초기화합니다.
	int i;
	
	for(i=0; i<256; i++)    charType[i]     = OTHERS;
	for(i='0'; i<='9'; i++) charType[i]     = DIGIT;
	for(i='A'; i<'Z'; i++)  charType[i]     = LETTER;
	for(i='a'; i<'z'; i++)  charType[i]     = LETTER;
	
	charType['_'] = LETTER;
	charType['('] = RBRK_1; charType[')'] = RBRK_2;
	charType['{'] = BRAC_1; charType['}'] = BRAC_2;
	charType['['] = SBRK_1; charType[']'] = SBRK_2;
	
}

Token nextToken() {
	TokenKind kind;
	int ch0, num = 0;
	static int ch = ' '; // static 이전문자 유지
	string text = "";
	
	while (isspace(ch)) {ch = nextToken();}
	if (ch == EOF) return Token(EOF, text);
	
	switch(charType[ch]) {
		case LETTER:
			while (charType[ch] == LETTER || charType[ch] == DIGIT) {
				text += ch;
				ch = nextChar();
			} break;
		
		case DIGIT:
			while (charType[ch] == DIGIT) {
				text += ch;
				ch = nextChar();
			}
			num = stoi(text);
			break;
			
	}
}

TokenKind getKind() {}
int nextChar() {}

void lexAnalysis(const string& filename) {
	
}