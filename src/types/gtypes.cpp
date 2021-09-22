#include "gtypes.hpp"

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
	{"\"", QUOTE}
};

Token token;