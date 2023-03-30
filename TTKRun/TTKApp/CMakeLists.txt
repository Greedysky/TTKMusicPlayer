cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKMusicPlayer)
project(${TARGET_NAME})

if(UNIX)
add_definitions(-DCONFIG_OUT_BUILD)
set(LIBRARY_OUTPUT_PATH ${MUSIC_INSTALL_DIR})
set(EXECUTABLE_OUTPUT_PATH ${MUSIC_INSTALL_DIR})
endif()

include_directories(${MUSIC_CONFIG_DIR})
include_directories(${MUSIC_COMMON_APP_DIR})

set(TTKAPPLICATION_HEADER_FILES
  ${MUSIC_COMMON_APP_DIR}/ttklocalpeer.h
  ${MUSIC_COMMON_APP_DIR}/ttkrunapplication.h
)

set(TTKAPPLICATION_SOURCE_FILES
  ${MUSIC_COMMON_APP_DIR}/ttklocalpeer.cpp
  ${MUSIC_COMMON_APP_DIR}/ttkrunapplication.cpp
)

set(HEADER_FILES
  ttkrunobject.h
  ${TTKAPPLICATION_HEADER_FILES}
)

set(SOURCE_FILES
  ttkrunobject.cpp
  ttkrunmain.cpp
  ${TTKAPPLICATION_SOURCE_FILES}
)

if(UNIX)
  set(HEADER_FILES
    ${MUSIC_CONFIG_DIR}/musicconfigobject.h
    ${MUSIC_CONFIG_DIR}/musicconfigdefine.h
    ${HEADER_FILES}
  )

  set(SOURCE_FILES
    ${MUSIC_CONFIG_DIR}/musicconfigobject.cpp
    ${SOURCE_FILES}
  )
endif()

SET(QRC_FILES
  ${MUSIC_UI_DIR}/MusicApp.qrc
)

if(WIN32)
  list(APPEND SOURCE_FILES TTKApp.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt5_add_resources(RCC_FILES ${QRC_FILES})

  add_executable(${TARGET_NAME} ${RCC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  list(APPEND QT5_LIBS Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets)
  if(WIN32)
    list(APPEND QT5_LIBS TTKConfig)
  endif()

  target_link_libraries(${TARGET_NAME} ${QT5_LIBS})
else()
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt4_add_resources(RCC_FILES ${QRC_FILES})
  
  add_executable(${TARGET_NAME} ${RCC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  list(APPEND QT4_LIBS ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY})
  if(WIN32)
    list(APPEND QT4_LIBS TTKConfig)
  endif()

  target_link_libraries(${TARGET_NAME} ${QT4_LIBS})
endif()
