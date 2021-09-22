#include <iostream>
#include <string>
#include <cstdlib>

#ifndef _ERRORS_HPP_
#define _ERRORS_HPP_


class Error{
	private:
		std::string errorName;
	
	public:
		Error();
		Error(const std::string& e_name);
		std::string getErrorName();
};

#endif


void error_exit(std::string msg);
void error_exit(Error *error, const std::string& msg);
