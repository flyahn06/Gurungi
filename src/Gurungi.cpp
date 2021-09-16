#include <iostream>
#include <string>
#include "lexer/lexer.hpp"
#include "utils/errors.hpp"

int main(int argc, char* args[]) {
	if (argc == 1) {
		error_exit("오류: 파일 이름이 없습니다. 자세한 내용은 \"Gurungi --도움\"을 참조하세요!");
	}
	
	std::string filename = args[1];
	
	std::cout << filename << " 파일을 인터프릿합니다." << std::endl
		<< "어휘 분석을 실행하는 중입니다...";
	
	
	

	return 0;
}