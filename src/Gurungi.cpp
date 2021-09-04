#include <iostream>
#include <string>
#include "lexer/lexer.hpp"
#include "utils/errors.hpp"

int main() {
	Error *error = new Error("SomeBadException");
	std::cout << "Hello, world!" << std::endl;
	error_exit(error, "some bad exception occured!");
	std::cout << "This message cannot be printed!" << std::endl;
	return 0;
}