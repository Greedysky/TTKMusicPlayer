cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKInit)
project(${TARGET_NAME})

include_directories(${MUSIC_CONFIG_DIR})

set(SOURCE_FILES
  musicinitmain.cpp
)

if(WIN32)
  list(APPEND SOURCE_FILES ${TARGET_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  add_executable(${TARGET_NAME} ${SOURCE_FILES})
  target_link_libraries(${TARGET_NAME} Qt5::Core TTKConfig)
else()
  add_executable(${TARGET_NAME} ${SOURCE_FILES})
  target_link_libraries(${TARGET_NAME} ${QT_QTCORE_LIBRARY} TTKConfig)
endif()
