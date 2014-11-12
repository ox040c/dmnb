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
    
file(GLOB h_main
    "*.hpp")

# ----------------------------------------------------------------------
# Setup link && include directories

include_directories(
  "/usr/local/lib"
  "/usr/lib64"
  
  "./Utility"
  "./API"
  "./CatalogManager"
  "./Interpreter"
  #TODO
)
        
# ----------------------------------------------------------------------
# Collect source files

set(dmnb_src "./")

file(GLOB SrcCpp
  "${dmnb_src}/*.cpp"

  "${dmnb_src}/API/*.cpp"
  "${dmnb_src}/CatalogManager/*.c*"
  "${dmnb_src}/Interpreter/*.cpp"
  # TODO
)

# ----------------------------------------------------------------------
# Create the executable

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x") # required for unrestricted unions
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -p") # add gprof information

add_library(dnmb_library  ${SrcCpp})

# ----------------------------------------------------------------------
# Setup required libraries

ADD_EXECUTABLE(main ${src_main} ${h_main})
target_link_libraries(main
   dnmb_library
   protobuf
)


