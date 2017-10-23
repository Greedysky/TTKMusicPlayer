cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKTest)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
    cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CACHEFILE_DIR}/bin/${TTK_VERSION})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CACHEFILE_DIR}/bin/${TTK_VERSION}) 

set(MUSIC_HEADERS
    musicautotest.h
    tst_codelines.h
  )

set(MUSIC_SOURCES
    tst_codelines.cpp
    musicapplicationmain.cpp
  )

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  
  find_package(Qt5Test REQUIRED)
  
  add_executable(${TARGET_NAME} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  target_link_libraries(${TARGET_NAME} Qt5::Core Qt5::Test Qt5::Gui Qt5::Widgets TTKCore)
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  
  set(QT_USE_QTTEST ON)
  
  add_executable(${TARGET_NAME} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  target_link_libraries(${TARGET_NAME} ${QT_QTGUI_LIBRARY} ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} TTKCore)
endif()
