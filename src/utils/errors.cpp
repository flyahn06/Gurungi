#include "errors.hpp"

using namespace std;

Error* LexerError = new Error("어휘 분석 오류");
Error* FuncDeclareError = new Error("함수 정의 오류");
Error* VarDeclareError = new Error("변수 정의 오류");


string Error::getErrorName() {
	return errorName;
}

Error::Error() {
	errorName = "BaseException";
}

Error::Error(const string& e_name) {
	errorName = e_name;
}

void error_exit(const string& msg) {
	cerr << RED << "오류! " << RESET
		 << msg << endl;
	exit(1);
}

void error_exit(const string& tempErrorName, const string& msg) {
	cerr << RED << tempErrorName << RESET << ": "
		 << msg << endl;
	exit(1);
}

void error_exit(Error *error, const string& msg) {
	cerr << RED << error->getErrorName() << RESET << ": " 
		 << msg << endl;
	exit(1);
}