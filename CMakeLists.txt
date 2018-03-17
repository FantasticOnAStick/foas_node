cmake_minimum_required(VERSION 2.8)
project(foas_node)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

if(NOT TARGET foas_common)
  add_subdirectory(../foas_common build/foas_common)
endif()

if(NOT TARGET foas_message)
  add_subdirectory(../foas_message build/foas_message)
endif()

if(NOT TARGET foas_plugin)
  add_subdirectory(../foas_plugin build/foas_plugin)
endif()

include_directories(
  include
  ../foas_common/include
  ../foas_common/3rdparty/json-c
  ../foas_message/include
  ../foas_plugin/include)

file(GLOB_RECURSE LIB_SRC FOLLOW_SYMLINKS src/node/*.cpp)
add_library(${PROJECT_NAME}
  ${LIB_SRC})

target_link_libraries(${PROJECT_NAME}
  foas_common
  foas_message
  foas_plugin)

add_executable(${PROJECT_NAME}-bin
  src/main.cpp)

target_link_libraries(${PROJECT_NAME}-bin
  foas_node)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

add_subdirectory(../plugins build/plugins)

enableGTesting()
