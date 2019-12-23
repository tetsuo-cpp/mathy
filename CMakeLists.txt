cmake_minimum_required(VERSION 3.9)
project(mathy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set(
  MATHY_SOURCE_FILES
  Lexer.cpp
  main.cpp
  )

add_executable(mathy ${MATHY_SOURCE_FILES})
