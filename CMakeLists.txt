cmake_minimum_required(VERSION 3.16)
project(codelithicEXE)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3ty/codelithic)
include_directories(3ty/codelithic/include)
include_directories(3ty/logs/)

add_executable(tests main.cpp)
TARGET_LINK_LIBRARIES(tests codelithic codelithic_web log4cplus)

#da linkare se si usa  3ty/log4cpp/logs.hpp  LINKER--> log4cplus