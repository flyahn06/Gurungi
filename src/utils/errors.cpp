#include "errors.hpp"

Error* LexerError = new Error("어휘 분석 오류");
Error* FuncDeclareError = new Error("함수 정의 오류");
Error* VarDeclareError = new Error("변수 정의 오류");
Error* BlockNotEndedError = new Error("블럭 정의 오류");
Error* UnexpectedTokenError = new Error("파싱 오류");
Error* SyntaxError = new Error("문법 오류");

std::string Error::getErrorName() {
	return errorName;
}

Error::Error() {
	errorName = "BaseException";
}

Error::Error(const std::string& e_name) {
	errorName = e_name;
}

void error_exit(const std::string& msg) {
	std::cerr << RED << "오류! " << RESET
		 << msg << std::endl;
	exit(1);
}

void error_exit(const std::string& tempErrorName, const std::string& msg) {
	std::cerr << RED << tempErrorName << RESET << ": "
		 << msg << std::endl;
	exit(1);
}

void error_exit(Error *error, const std::string& msg) {
	std::cerr << RED << error->getErrorName() << RESET << ": "
		 << msg << std::endl;
	exit(1);
}