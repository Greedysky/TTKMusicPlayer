cmake_minimum_required(VERSION 2.8.11)

project(TTKInit)

include_directories(${MUSIC_CONFIG_DIR})

set(SOURCE_FILES
  musicinitmain.cpp
)

if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  add_executable(${PROJECT_NAME} ${SOURCE_FILES})
  target_link_libraries(${PROJECT_NAME} Qt5::Core TTKConfig)
else()
  add_executable(${PROJECT_NAME} ${SOURCE_FILES})
  target_link_libraries(${PROJECT_NAME} ${QT_QTCORE_LIBRARY} TTKConfig)
endif()
