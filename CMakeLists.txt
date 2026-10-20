cmake_minimum_required(VERSION 3.16)
project(CompileCTry LANGUAGES CXX)

add_executable(compile_c_try
    src/token.cpp
    src/keywords.cpp
    src/literal_scanner.cpp
    src/scanner_utils.cpp
    src/scanner.cpp
    src/parser.cpp
    src/main.cpp
)

target_include_directories(compile_c_try PRIVATE include)
target_compile_features(compile_c_try PRIVATE cxx_std_17)
set_target_properties(compile_c_try PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
