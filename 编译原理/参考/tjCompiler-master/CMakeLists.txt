# 设置cmake最低版本
cmake_minimum_required(VERSION 3.10)
    
# 设置编译器
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
    
# 设置项目名
project(youmengCC)
    
SET(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/run)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# 将输入文件复制到构建目录
configure_file(${CMAKE_SOURCE_DIR}/test.txt ${CMAKE_BINARY_DIR}/test.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/TestWrong1.txt ${CMAKE_BINARY_DIR}/TestWrong1.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/TestWrong2.txt ${CMAKE_BINARY_DIR}/TestWrong2.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/TestWrong3.txt ${CMAKE_BINARY_DIR}/TestWrong3.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/grammar.txt ${CMAKE_BINARY_DIR}/grammar.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/WordParseGrammar.txt ${CMAKE_BINARY_DIR}/WordParseGrammar.txt COPYONLY)



# 添加可执行目标
add_executable(main common.cpp grammarParse.cpp main.cpp myParse.cpp ObjectCodeGen.cpp)
