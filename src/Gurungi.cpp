#include <iostream>
#include <string>
#include <vector>
#include "lexer/lexer.hpp"
#include "utils/errors.hpp"
#include "arg_parser/arg_parser.hpp"

using namespace std;

int main(int argc, char* args[]) {
	if (argc == 1) {
		error_exit("오류: 파일 이름이 없습니다. 자세한 내용은 \"Gurungi --도움\"을 참조하세요!");
	}
	
	string filename = args[1];
    fstream fin;

    fin.open(filename);
    if (!fin.is_open()) error_exit("파일을 열 수 없습니다.");

    string source;
    while (!fin.eof()) source += fin.get();
	source.resize(source.length() - 1); // 맨 마지막 EOF 거르기용
	
	cout << filename << " 파일을 인터프릿합니다." << endl
		<< "어휘 분석을 실행하는 중입니다..." << endl;
	lexAnalysis(source);
	
	return 0;
}