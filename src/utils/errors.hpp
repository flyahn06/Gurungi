#include <iostream>
#include <string>
#include <cstdlib>

class Error{
	private:
		std::string errorName;
	
	public:
		Error();
		Error(const std::string& e_name);
		std::string getErrorName();
};

void error_exit(std::string msg);
void error_exit(Error *error, const std::string& msg);
