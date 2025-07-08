cmake_minimum_required(VERSION 3.10)
project(truth_table_generator)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(truth-table-generator
    src/main.cpp
    stc/parser.cpp
    src/evaluator.cpp
    src/truth_table.cpp
)

set_target_properties(truth-table-generator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
