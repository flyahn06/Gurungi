cmake_minimum_required(VERSION 3.5)
project(Gurungi)

# Set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(Gurungi
        src/Gurungi.cpp
        src/arg_parser/arg_parser.cpp
        src/arg_parser/arg_parser.hpp
        src/lexer/lexer.cpp
        src/lexer/lexer.hpp
        src/parser/parser.cpp
        src/parser/parser.hpp
        src/static_code_analysis/analysis.cpp
        src/static_code_analysis/analysis.hpp
        src/types/gtypes.cpp
        src/types/gtypes.hpp
        src/utils/errors.cpp
        src/utils/errors.hpp
        src/utils/gwrappers.cpp
        src/utils/gwrappers.hpp
        )

# Set include directories
target_include_directories(Gurungi PRIVATE
        src/arg_parser
        src/lexer
        src/parser
        src/static_code_analysis
        src/types
        src/utils)
