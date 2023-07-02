cmake_minimum_required(VERSION 3.10)
project(huffman)

set(CMAKE_CXX_STANDARD 20)
set(PEDANTIC_FLAGS -Wall -Wextra -Werror -pedantic)
include_directories(extern include)

set(SRC_LIST src/Application.cpp src/HuffmanArchiver.cpp src/Reader.cpp src/Writer.cpp src/Tree.cpp)
add_executable(${PROJECT_NAME} src/main.cpp ${SRC_LIST})
target_compile_options(${PROJECT_NAME} PRIVATE ${CMAKE_CXX_FLAGS} ${PEDANTIC_FLAGS} -O2)
add_executable(tests test/test.cpp ${SRC_LIST})
target_compile_options(tests PRIVATE ${CMAKE_CXX_FLAGS} ${PEDANTIC_FLAGS} -g)