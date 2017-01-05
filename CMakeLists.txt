cmake_minimum_required(VERSION 3.5)
project (rubik)
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-c++14-extensions -Wall -Wextra -g" )
SET( CMAKE_EXE_LINKER_FLAGS "-ltermcap")
add_executable (rubik
      CliOptParser.cpp
      Database.cpp
      Heuristics.cpp
      Parser.cpp
      Solver.cpp
      State.cpp
      Types.cpp
      main.cpp
      tools.cpp)
