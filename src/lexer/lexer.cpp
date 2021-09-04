#include "lexer.hpp"

TokenKind TokenType[256];

void initTokenType(){
	// TokenType 배열을 초기화합니다.
	int i;
	
	for(i=0; i<256; i++)    TokenType[i]     = OTHERS;
	for(i='0'; i<='9'; i++) TokenType[i]     = DIGIT;
	for(i='A'; i<'Z'; i++)  TokenType[i]     = LETTER;
	for(i='a'; i<'z'; i++)  TokenType[i]     = LETTER;
	
}