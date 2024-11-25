#include <iostream>
#include <string>
#include "Gurungi.hpp"

std::string filename;

int main(int argc, char* args[]) {

	if (argc == 1) {
		error_exit("파일 이름이 없습니다. 자세한 내용은 \"Gurungi --도움\"을 참조하세요!");
	}
	
	filename = args[1];
    std::fstream fin;

    fin.open(filename);
    if (!fin.is_open()) error_exit("파일을 열 수 없습니다.");

    std::string source;
    while (!fin.eof()) source += fin.get();
	source.resize(source.length() - 1); // 맨 마지막 EOF 거르기용

    std::cout << filename << " 파일을 인터프릿합니다." << std::endl
		<< "어휘 분석을 실행하는 중입니다..." << std::endl;
	
	initLexAnalysis(source);
	lexAnalysisDebug();
	std::cout << "어휘 분석을 마쳤습니다." << std::endl;

	resetPointer();
	std::cout << "파싱중입니다..." << std::endl;
	parseIntercode();
	std::cout << "파싱을 마쳤습니다. " << std::endl;

	std::cout << "실행을 시작합니다." << std::endl;
	initialize_executor();
	execute();

	return 0;
}
