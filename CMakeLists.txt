cmake_minimum_required(VERSION 3.5.0)
project(testboost)

INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
IF(COMPILER_SUPPORTS_CXX11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSEIF(COMPILER_SUPPORTS_CXX0X)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
ELSE()
  MESSAGE(ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
ENDIF()

find_package(Boost REQUIRED COMPONENTS system thread regex)

include_directories(${Boost_INCLUDE_DIRS})

#add_library(src/judge.cpp myJudge)

add_library(boolExpression src/boolExpression.cpp)
add_library(basic src/basic.cpp)
add_library(AST src/AST.cpp)
add_executable(minipascal src/main.cpp)
target_link_libraries(minipascal ${Boost_LIBRARIES} boolExpression basic AST)

