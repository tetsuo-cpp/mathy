cmake_minimum_required(VERSION 3.9)
project(mathy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(mathy)
