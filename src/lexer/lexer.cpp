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