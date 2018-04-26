# SET( CC /usr/bin/clang )
# SET( CXX /usr/bin/clang++ )

cmake_minimum_required(VERSION 3.5)
project (rubik)

SET( OPTI "-Ofast -march=native -funroll-loops -fprofile-generate")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra" )
SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -pg" )
SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPTI}" )

SET( CMAKE_EXE_LINKER_FLAGS "-ltermcap -lncurses -fprofile-generate")
SET( CMAKE_EXE_LINKER_FLAGS_DEBUG "-pg")

add_executable (rubik
      OptParser.cpp
      Database.cpp
      Parser.cpp
      Solver.cpp
      main.cpp
      tools.cpp
      State_Const.cpp) 
