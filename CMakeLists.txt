cmake_minimum_required(VERSION 3.16)
project(codelithicEXE)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3ty/codelithic)
include_directories(3ty/codelithic/include)


add_executable(codelithicEXE main.cpp)
TARGET_LINK_LIBRARIES(codelithicEXE codelithic codelithic_web)

