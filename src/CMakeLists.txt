# This is a CMake build file, for more information consult:
# http://en.wikipedia.org/wiki/CMake
# and
# http://www.cmake.org/Wiki/CMake
# http://www.cmake.org/cmake/help/syntax.html
# http://www.cmake.org/Wiki/CMake_Useful_Variables
# http://www.cmake.org/cmake/help/cmake-2-8-docs.html

# to compile the local code you can use: cmake ./ && make -j2

# ----------------------------------------------------------------------
# Base CMake setup
cmake_minimum_required (VERSION 2.6)

project(dmnb)


#-------------------------- add common files 	-------------------------
SET(src_main
    "./main.cpp")
    
#file(GLOB h_main
#    "./*.hpp")

# ----------------------------------------------------------------------
# Setup link && include directories

link_directories(
    "/usr/local/lib"
  "/usr/lib64"
)

include_directories(
  "/usr/local/include"
  
  "./API"
  "./BufferManager"
  "./CatalogManager"
  "./IndexManager"
  "./Interpreter"
  "./RecordManager"
  "./Utility"
  
  #TODO
)
        
# ----------------------------------------------------------------------
# Collect source files

set(dmnb_src "./")

file(GLOB SrcList

  "${dmnb_src}/API/*.cpp"
  "${dmnb_src}/API/*.hpp"
  "${dmnb_src}/BufferManager/*.cpp"
  "${dmnb_src}/BufferManager/*.hpp"
  "${dmnb_src}/CatalogManager/*.c*"
  "${dmnb_src}/CatalogManager/*.h*"
  "${dmnb_src}/IndexManager/*.cpp"
  "${dmnb_src}/IndexManager/*.hpp"
  "${dmnb_src}/Interpreter/lex.yy.cpp"
  "${dmnb_src}/Interpreter/parse.tab.cpp"
  #"${dmnb_src}/Interpreter/*.c*"
  #"${dmnb_src}/Interpreter/*.h*"
  "${dmnb_src}/RecordManager/*.cpp"
  "${dmnb_src}/RecordManager/*.hpp"
  "${dmnb_src}/Utility/*.cpp"
  "${dmnb_src}/Utility/*.hpp"
  
  # TODO
)

# ----------------------------------------------------------------------
# Create the executable & Setup required libraries


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x") # required for unrestricted unions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o2")
# add gprof information

ADD_EXECUTABLE(main ${src_main} ${SrcList})
target_link_libraries(main
   protobuf
)


