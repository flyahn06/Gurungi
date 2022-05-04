#include "gtypes.hpp"

using namespace std;

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

vector<string> TokenKindMap;


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
	cout
	<< "name: " << name << endl
	<< "kind: " << kind << endl
	<< "type: " << type << endl
	<< "arrayLength: " << arrayLength << endl
	<< "args: " << args << endl
	<< "address: " << address << endl
	<< "frame: " << frame << endl;
}

Token token;

void initialize() {
    string Tokens[] = {
            "RBRK_1", "RBRK_2", "BRAC_1", "BRAC_2", "SBRK_1", "SBRK_2",

            "PLUS", "MINUS", "STAR", "SLASH",
            "LESS", "LESSEQ", "GREAT", "GREATEQ", "EQUAL", "NOTEQUAL",

            "ASSIGN", "COMMA", "DOT", "QUOTE",

            "PRINT",
            "IF", "ELSE", "ELIF", "WHILE", "FOR",
            "FUNC", "VAR",
            "END",

            "DIGIT", "LETTER", "INTVALUE", "DOUBLEVALUE",

            "IDENTIFIER", "NUMBER", "STRING", "OTHERS",
            "EOF_TOKEN"
    };

    for (int i=0; i < Tokens->size() / sizeof(string); i++) {
        TokenKindMap.push_back(Tokens[i]);
    }
}
