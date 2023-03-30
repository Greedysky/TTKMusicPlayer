cmake_minimum_required(VERSION 2.8.11)

include_directories(.)
include_directories(TTKApp)
include_directories(TTKInit)

add_subdirectory(TTKInit)
add_subdirectory(TTKApp)
add_subdirectory(TTKConsole)
add_subdirectory(TTKTools)
