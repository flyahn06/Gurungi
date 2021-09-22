#include "gtypes.hpp"

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
// 	Token(TokenKind k, const std::string& s, int d=0) {
// 		kind = k;
// 		text = s;
// 		intValue = 0;
// 	}
	
// };

// struct KeyWord {
// 	std::string keywordName;
// 	TokenKind keywordKind;
// };

// KeyWord keyWordTable[] = {
// 	{"만약", IF}, {"아니라면", ELSE},
// 	{"끝", END}, {"프린트", PRINT},
// 	{"(", RBRK_1}, {")", RBRK_2},
// 	{"{", BRAC_1}, {"}", BRAC_2},
// 	{"[", SBRK_1}, {"]", SBRK_2},
// 	{"+", PLUS}, {"-", MINUS},
// 	{"*", STAR}, {"/", SLASH},
// 	{"=", ASSIGN}, {",", COMMA},
// 	{"==", EQUAL}, {"/=", NOTEQUAL},
// 	{"<", LESS}, {">", GREAT},
// 	{"<=", LESSEQ}, {">=", GREATEQ},
// 	{"\"", QUOTE}
// };