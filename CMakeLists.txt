cmake_minimum_required(VERSION 3.5)
project (rubik)
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-c++14-extensions -Wall -Wextra -Ofast" )
SET( CMAKE_EXE_LINKER_FLAGS "-ltermcap")
add_executable (rubik
      CliOptParser.cpp
      Database.cpp
      Parser.cpp
      Solver.cpp
      Types.cpp
      main.cpp
      tools.cpp)
