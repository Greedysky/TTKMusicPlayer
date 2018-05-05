cmake_minimum_required(VERSION 2.8.11)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
    cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

include_directories(.)
include_directories(TTKApp)
include_directories(TTKInit)

add_subdirectory(TTKInit)
add_subdirectory(TTKApp)
add_subdirectory(TTKConsole)
add_subdirectory(TTKTools)