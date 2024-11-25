#ifndef _ERRORS_HPP_
#define _ERRORS_HPP_

#include <iostream>
#include <string>
#include "../Gurungi.hpp"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


class Error{
	private:
		std::string errorName;
	
	public:
		Error();
		Error(const std::string& e_name);
		std::string getErrorName();
};



extern Error* LexerError;
extern Error* FuncDeclareError;
extern Error* VarDeclareError;
extern Error* BlockNotEndedError;
extern Error* UnexpectedTokenError;
extern Error* SyntaxError;
extern Error* RuntimeError;

void error_exit(const std::string& msg);
void error_exit(const std::string& tempErrorName, const std::string& msg);
void error_exit(Error *error, const std::string& msg);
void error_exit(int line, const std::string& msg);
void error_exit(const std::string& tempErrorName, int line, const std::string& msg);
void error_exit(Error *error, int line, const std::string& msg);

#endif