#include "gtypes.hpp"

KeyWord keyWordTable[] = {
	{"만약", IF}, {"아니라면", ELSE},
	{"끝", END}, {"출력", PRINT},
	{"함수", FUNC}, {"변수", VAR},
	{"(", RBRK_1}, {")", RBRK_2},
	{"{", BRAC_1}, {"}", BRAC_2},
	{"[", SBRK_1}, {"]", SBRK_2},
	{"+", PLUS}, {"-", MINUS},
	{"*", STAR}, {"/", SLASH},
	{"=", ASSIGN}, {",", COMMA},
	{"==", EQUAL}, {"/=", NOTEQUAL},
	{"<", LESS}, {">", GREAT},
	{"<=", LESSEQ}, {">=", GREATEQ},
	{"\"", QUOTE}, {"\n", OTHERS}
};

void SymbolTable::clear() {
	name = "";
	kind = noID;
	type = noType;
	arrayLength = 0;
	args = 0;
	address = 0;
	frame = 0;
}

Token token;