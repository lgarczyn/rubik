cmake_minimum_required( VERSION 3.5 )
project( rubik )

SET( OPTI "-Ofast -march=native -funroll-loops")
#      -fwhole-program
#      -flto


SET( OPTI_LINKER
#      -flto
#      -fprofile-use=profile
#      -fprofile-generate=profile
)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -Wextra" )
SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}   -ftrapv -g" )
SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPTI}" )

SET( CMAKE_EXE_LINKER_FLAGS "" )
SET( CMAKE_EXE_LINKER_FLAGS_DEBUG "" )
SET( CMAKE_EXE_LINKER_FLAGS_RELEASE "${OPTI_LINKER}" )

add_executable( rubik main.cpp )
