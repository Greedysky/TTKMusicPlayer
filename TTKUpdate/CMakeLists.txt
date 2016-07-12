cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKUpdate)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_UPDATE_CORE_DIR "${MUSIC_TTKUPDATE_DIR}/musicCore")
set(MUSIC_UPDATE_WIDGET_DIR "${MUSIC_TTKUPDATE_DIR}/musicWidget")
set(MUSIC_UPDATE_UI_DIR "${MUSIC_TTKUPDATE_DIR}/musicUi")
set(MUSIC_UPDATE_ZIP_DIR "${MUSIC_TTKUPDATE_DIR}/musicZip")

include_directories(${MUSIC_TTKUPDATE_DIR})
include_directories(${MUSIC_UPDATE_CORE_DIR})
include_directories(${MUSIC_UPDATE_WIDGET_DIR})
include_directories(${MUSIC_UPDATE_UI_DIR})
include_directories(${MUSIC_UPDATE_ZIP_DIR})


set(MUSIC_HEADERS
    ${MUSIC_UPDATE_ZIP_DIR}/crypt.h
    ${MUSIC_UPDATE_ZIP_DIR}/ioapi.h
    ${MUSIC_UPDATE_ZIP_DIR}/quazip.h
    ${MUSIC_UPDATE_ZIP_DIR}/quazipfile.h
    ${MUSIC_UPDATE_ZIP_DIR}/quazipfileinfo.h
    ${MUSIC_UPDATE_ZIP_DIR}/quazipnewinfo.h
    ${MUSIC_UPDATE_ZIP_DIR}/unzip.h
    ${MUSIC_UPDATE_ZIP_DIR}/zconf.h
    ${MUSIC_UPDATE_ZIP_DIR}/zip.h
    ${MUSIC_UPDATE_ZIP_DIR}/gojobzip.h
    ${MUSIC_UPDATE_ZIP_DIR}/zlib.h
    ${MUSIC_UPDATE_CORE_DIR}/musicupdateglobaldefine.h
    ${MUSIC_UPDATE_CORE_DIR}/musicupdateunzipthread.h
    ${MUSIC_UPDATE_WIDGET_DIR}/musicabstractmovedialog.h
    ${MUSIC_UPDATE_WIDGET_DIR}/musicabstractmovewidget.h
    ${MUSIC_UPDATE_WIDGET_DIR}/musicmessagebox.h
    ${MUSIC_UPDATE_WIDGET_DIR}/musicprogresswidget.h
    ${MUSIC_TTKUPDATE_DIR}/musicupdatemainwindow.h
)

set(MUSIC_SOURCES
    ${MUSIC_UPDATE_ZIP_DIR}/ioapi.c
    ${MUSIC_UPDATE_ZIP_DIR}/quazip.cpp
    ${MUSIC_UPDATE_ZIP_DIR}/quazipfile.cpp
    ${MUSIC_UPDATE_ZIP_DIR}/quazipnewinfo.cpp
    ${MUSIC_UPDATE_ZIP_DIR}/gojobzip.cpp
    ${MUSIC_UPDATE_ZIP_DIR}/unzip.c
    ${MUSIC_UPDATE_ZIP_DIR}/zip.c
    ${MUSIC_UPDATE_CORE_DIR}/musicupdateunzipthread.cpp
    ${MUSIC_UPDATE_WIDGET_DIR}/musicabstractmovedialog.cpp
    ${MUSIC_UPDATE_WIDGET_DIR}/musicabstractmovewidget.cpp
    ${MUSIC_UPDATE_WIDGET_DIR}/musicmessagebox.cpp
    ${MUSIC_UPDATE_WIDGET_DIR}/musicprogresswidget.cpp
    ${MUSIC_TTKUPDATE_DIR}/musicupdatemainwindow.cpp
    ${MUSIC_TTKUPDATE_DIR}/musicapplicationmain.cpp
)

SET(MUSIC_UIS
    ${MUSIC_UPDATE_UI_DIR}/musicmessagebox.ui
)

SET(MUSIC_QRC
    ${MUSIC_QRC_DIR}/MusicUpdate.qrc
)

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_UI(MUSIC_UIS_H ${MUSIC_UIS})
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  QT5_ADD_RESOURCES(MUSIC_RCC_SRCS ${MUSIC_QRC})
  
  add_executable(${TARGET_NAME} TTKUpdate.rc ${MUSIC_SOURCES} ${MUSIC_RCC_SRCS} ${MUSIC_UIS_H} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  list(APPEND QT5_LIBS Qt5::Core Qt5::Gui Qt5::Widgets zlib)
  target_link_libraries(${TARGET_NAME} ${QT5_LIBS})
else()
  QT4_WRAP_UI(MUSIC_UIS_H ${MUSIC_UIS})
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  QT4_ADD_RESOURCES(MUSIC_RCC_SRCS ${MUSIC_QRC})
  
  add_executable(${TARGET_NAME} TTKUpdate.rc ${MUSIC_SOURCES} ${MUSIC_RCC_SRCS} ${MUSIC_UIS_H} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  list(APPEND QT4_LIBS ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} zlib)
  target_link_libraries(${TARGET_NAME} ${QT4_LIBS})
endif()
