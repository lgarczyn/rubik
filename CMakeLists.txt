SET( CC /usr/bin/clang )
SET( CXX /usr/bin/clang++ )

cmake_minimum_required(VERSION 3.5)
project (rubik)
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra -Ofast" )
SET( CMAKE_EXE_LINKER_FLAGS "-ltermcap -lncurses")
add_executable (rubik
      OptParser.cpp
      Database.cpp
      Parser.cpp
      Solver.cpp
      main.cpp
      tools.cpp
      State_Const.cpp) 
