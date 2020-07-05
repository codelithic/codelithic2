cmake_minimum_required(VERSION 3.16)
project(codelithicEXE)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3ty/codelithic)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/3ty/codelithic/codelithic/include)

include_directories(3ty/logs/)


add_executable(tests main.cpp)
TARGET_LINK_LIBRARIES(tests codelithic codelithic_web log4cplus)


SET(THE_SITE
        src/site/main.cpp
        )

add_executable(site ${THE_SITE})
TARGET_LINK_LIBRARIES(site codelithic codelithic_site log4cplus)


#da linkare se si usa  3ty/log4cpp/logs.hpp  LINKER--> log4cplus