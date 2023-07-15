cmake_minimum_required(VERSION 2.8.11)

project(TTKService)

include_directories(${MUSIC_CONFIG_DIR})
include_directories(${MUSIC_COMMON_APP_DIR})
include_directories(${MUSIC_COMMON_DUMPER_DIR})

set(TTKAPPLICATION_HEADER_FILES
  ${MUSIC_COMMON_APP_DIR}/ttklocalpeer.h
  ${MUSIC_COMMON_APP_DIR}/ttkrunapplication.h
)

set(TTKAPPLICATION_SOURCE_FILES
  ${MUSIC_COMMON_APP_DIR}/ttklocalpeer.cpp
  ${MUSIC_COMMON_APP_DIR}/ttkrunapplication.cpp
)

set(HEADER_FILES
  ${TTKAPPLICATION_HEADER_FILES}
)

set(SOURCE_FILES
  musicservicemain.cpp
  ${TTKAPPLICATION_SOURCE_FILES}
)

if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  
  add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  add_dependencies(${PROJECT_NAME} TTKCore)
  target_link_libraries(${PROJECT_NAME} Qt5::Core TTKCore TTKConfig)
else()
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})

  add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  add_dependencies(${PROJECT_NAME} TTKCore)
  target_link_libraries(${PROJECT_NAME} ${QT_QTCORE_LIBRARY} TTKCore TTKConfig)
endif()
