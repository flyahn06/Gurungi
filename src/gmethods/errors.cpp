#include "errors.hpp"

using namespace std;

string Error::getErrorName() {
	return errorName;
}

Error::Error() {
	errorName = "BaseException";
}

Error::Error(const string& e_name) {
	errorName = e_name;
}

void error_exit(string msg) {
	cout << msg << endl;
	exit(1);
}

void error_exit(Error *error, const string& msg) {
	cout << error->getErrorName() << ": " << msg << endl;
	exit(1);
}