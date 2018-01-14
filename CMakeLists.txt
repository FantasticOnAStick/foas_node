cmake_minimum_required(VERSION 2.8)
project(foas_node)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

add_subdirectory(../foas_common build/foas_common)

include_directories(
  include
  ../foas_common/include)

file(GLOB_RECURSE LIB_SRC src/node FOLLOW_SYMLINKS *.cpp)
add_library(${PROJECT_NAME}
  ${LIB_SRC})

target_link_libraries(${PROJECT_NAME}
  foas_common)

add_executable(${PROJECT_NAME}-bin
  src/main.cpp)

target_link_libraries(${PROJECT_NAME}-bin
  foas_node)
