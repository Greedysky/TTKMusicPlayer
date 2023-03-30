cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKConsole)
project(${TARGET_NAME})

set(HEADER_FILES
  musicconsolemodule.h
)

set(SOURCE_FILES
  musicconsolemodule.cpp
  musicconsolemain.cpp
)

if(WIN32)
  list(APPEND SOURCE_FILES ${TARGET_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  
  add_executable(${TARGET_NAME} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  target_link_libraries(${TARGET_NAME} Qt5::Core TTKCore)
else()
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  
  add_executable(${TARGET_NAME} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  target_link_libraries(${TARGET_NAME} ${QT_QTCORE_LIBRARY} TTKCore)
endif()
