cmake_minimum_required(VERSION 2.8.11)

project(MusicWidget)

set(MUSIC_Widget_HEADERS
    ${MUSIC_Widget_DIR}/*.h
    ${MUSIC_Localsearch_DIR}/*.h
    ${MUSIC_Lrcmanager_DIR}/*.h
    ${MUSIC_Remotewidget_DIR}/*.h
    ${MUSIC_Toolswidget_DIR}/*.h
    ${MUSIC_Toolsetswidget_DIR}/*.h
    ${MUSIC_Usermanager_DIR}/*.h
    ${MUSIC_Videokits_DIR}/*.h
    ${MUSIC_DIR}/*.h
  )

set(MUSIC_Widget_SOURCES
    ${MUSIC_Widget_DIR}/*.cpp
    ${MUSIC_Localsearch_DIR}/*.cpp
    ${MUSIC_Lrcmanager_DIR}/*.cpp
    ${MUSIC_Remotewidget_DIR}/*.cpp
    ${MUSIC_Toolswidget_DIR}/*.cpp
    ${MUSIC_Toolsetswidget_DIR}/*.cpp
    ${MUSIC_Usermanager_DIR}/*.cpp
    ${MUSIC_Videokits_DIR}/*.cpp
    ${MUSIC_DIR}/*.cpp
  )

set(MUSIC_Widget_UI
    ${MUSIC_Ui_DIR}/application/musicapplication.ui
    ${MUSIC_Ui_DIR}/musicLrcmanager/musiclrcartphotoupload.ui
    ${MUSIC_Ui_DIR}/musicLrcmanager/musiclrcmakerwidget.ui
    ${MUSIC_Ui_DIR}/musicLrcmanager/musiclrcsearchwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musicaudiorecorderwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musicdesktopwallpaperwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musicfileinformationwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musiclocalsongsmanagerwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musictimerwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musictransformwidget.ui
    ${MUSIC_Ui_DIR}/musicToolswidget/musicwebradiowidget.ui
    ${MUSIC_Ui_DIR}/musicUsermanager/musicuserdialog.ui
    ${MUSIC_Ui_DIR}/musicUsermanager/musicusermanager.ui
    ${MUSIC_Ui_DIR}/musicUsermanager/musicuserwindow.ui
    ${MUSIC_Ui_DIR}/musicUsermanager/musicuserrecordwidget.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicbackgroundskindialog.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicdownloadwidget.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicequalizerdialog.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicmessagebox.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicsettingwidget.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicsongsharingwidget.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicsongslistiteminfowidget.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicsourceupdatewidget.ui
  )

include_directories(${MUSIC_Core_DIR})
include_directories(${MUSIC_Core_DIR}/network)
include_directories(${MUSIC_Localsearch_DIR}/core)
include_directories(${MUSIC_Toolswidget_DIR}/core)
include_directories(${MUSIC_Toolsetswidget_DIR}/core)
include_directories(${MUSIC_Usermanager_DIR}/core)

qt5_wrap_cpp(MUSIC_Widget_SOURCES_MOC ${MUSIC_Widget_HEADERS})
qt5_wrap_ui(MUSIC_UI_HEADERS ${MUSIC_Widget_UI})

qt5_add_resources(MUSIC_QRCS ${MUSIC_QRC_DIR}/*.qrc)

add_library(MusicWidget SHARED ${MUSIC_Widget_HEADERS} ${MUSIC_Widget_SOURCES} ${MUSIC_Widget_SOURCES_MOC} ${MUSIC_UI_HEADERS} ${MUSIC_QRCS} )
target_link_libraries(MusicWidget ${CMAKE_CACHEFILE_DIR}/bin/Release/MusicCore.lib)

qt5_use_modules(MusicWidget Core Network Sql Gui Widgets Multimedia MultimediaWidgets WinExtras)