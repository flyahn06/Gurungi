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
        {"\"", QUOTE}, {"\n", NEW_LINE}
};

std::vector<std::string> TokenKindMap = {
        "RBRK_1", "RBRK_2", "BRAC_1", "BRAC_2", "SBRK_1", "SBRK_2",
        "PLUS", "MINUS", "STAR", "SLASH",
        "LESS", "LESSEQ", "GREAT", "GREATEQ", "EQUAL", "NOTEQUAL",
        "ASSIGN", "COMMA", "DOT", "QUOTE",
        "PRINT",
        "IF", "ELSE", "ELIF", "WHILE", "FOR",
        "FUNC", "VAR",
        "END",
        "DIGIT", "LETTER", "INTVALUE", "DOUBLEVALUE",
        "IDENTIFIER", "NUMBER", "STRING", "OTHERS", "NEW_LINE",
        "EOF_TOKEN"
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

void SymbolTable::printElements() const {
	std::cout
	<< "name: " << name << std::endl
	<< "kind: " << kind << std::endl
	<< "type: " << type << std::endl
	<< "arrayLength: " << arrayLength << std::endl
	<< "args: " << args << std::endl
	<< "address: " << address << std::endl
	<< "frame: " << frame << std::endl;
}

Token token;