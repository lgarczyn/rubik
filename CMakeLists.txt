SET( CC /usr/bin/gcc )
SET( CXX /usr/bin/g++ )
SET( AR /usr/bin/ar )
SET( NM /usr/bin/nm )
SET( RANLIB /bin/true )

cmake_minimum_required( VERSION 3.5 )
project( rubik )

SET( OPTI
      -Ofast 
      -march=native 
      -funroll
      -loops 
#      -fwhole-program
#      -flto
)

SET( OPTI_LINKER
#      -fwhole-program
#      -flto
#      -fprofile-use
#      -fprofile-generate
)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra" )
SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}   -ftrapv" )
SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPTI}" )

SET( CMAKE_EXE_LINKER_FLAGS "-ltermcap -lncurses" )
SET( CMAKE_EXE_LINKER_FLAGS_DEBUG "" )
SET( CMAKE_EXE_LINKER_FLAGS_RELEASE "${OPTI_LINKER}" )

add_executable( rubik
      OptParser.cpp
      Database.cpp
      Parser.cpp
      Solver.cpp
      main.cpp
      tools.cpp
      State_Const.cpp ) 
