#include "executor.hpp"

unsigned int pc = 0;  // intercode상의 위치
unsigned int current_line = 0;
unsigned int previous_line = 0;
unsigned int max_line = 0;

void initialize_executor() {
    for (auto k: intercode) {
        if (k == "\n") max_line++;
    }
    std::cout << "max_line: " << max_line << std::endl;
}

std::string construct_error_line() {
    std::string error_line;
    bool isLiteral = false;
    int tmp;
    int line = 1;

    for (auto & code : intercode) {
        std::cout << " ";

        if (code == "\n") {
            std::cout << "\nln " << line << ":";
            line++;
            continue;
        }

        if(isLiteral) {
            isLiteral = false;
            std::cout << code;
        } else if(code[0] == '[' && code[code.length()-1] == ']') {
            try {
                tmp = stoi(code.substr(1, code.length() - 2));
            } catch (const std::exception &e) {
                std::cout << code;
                continue;
            }
            if (tmp == NUMBER || tmp == STRING) {
                isLiteral = true;
            }
            std::cout << "[" << TokenKindMap[tmp] << "]";
        } else if (code == "GVar") {
            std::cout << "[GVar]";
        } else if (code == "LVar") {
            std::cout << "[LVar]";
        } else if (code == "CallFunc") {
            std::cout << "[CallFunc]";
        } else {
            std::cout << code;
        }
    }
    std::cout << std::endl;
}

void jump(unsigned int new_line) {
    if (new_line > max_line || new_line <= 0) {
        error_exit(RuntimeError, std::to_string(new_line) + " 라인으로는 점프할 수 없습니다. (Max: " + std::to_string(max_line) + ")");
    }
    int line = 1, idx = 0;
    for (const auto& k: intercode) {
        if (k == "\n") line++;

        if (line == new_line) {
            pc = idx;
            previous_line = current_line;
            current_line = new_line;
            break;
        }
        idx++;
    }
}

void execute() {
    jump(0);
}