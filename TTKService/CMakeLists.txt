cmake_minimum_required(VERSION 2.8.11)

project(TTKService)

include_directories(${MUSIC_CONFIG_DIR})
include_directories(${MUSIC_COMMON_DUMPER_DIR})

set(SOURCE_FILES
  musicservicemain.cpp
)

if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} TTKCore)

if(TTK_QT_VERSION VERSION_GREATER "4")
  target_link_libraries(${PROJECT_NAME} Qt5::Core TTKCore TTKConfig)
else()
  target_link_libraries(${PROJECT_NAME} ${QT_QTCORE_LIBRARY} TTKCore TTKConfig)
endif()
