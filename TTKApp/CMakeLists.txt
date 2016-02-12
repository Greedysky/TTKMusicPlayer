cmake_minimum_required(VERSION 2.8.11)

project(TTKMusicPlayer)
set(TARGET_NAME TTKMusicPlayer)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_THREAD)

set(MUSIC_APP_SOURCES
  musicapplicationmain.cpp
)

ADD_EXECUTABLE(${TARGET_NAME} TTKApp.rc ${MUSIC_APP_SOURCES})
add_dependencies(${TARGET_NAME} MusicCore)
if(TTK_QT_VERSION VERSION_GREATER "4")
  if(WIN32)
    target_link_libraries(${TARGET_NAME} Qt5::Core Qt5::Network Qt5::Sql Qt5::Xml Qt5::Gui Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets Qt5::WinExtras ${QMMP_LIBRARY} MusicCore)
  else()
    target_link_libraries(${TARGET_NAME} Qt5::Core Qt5::Network Qt5::Sql Qt5::Xml Qt5::Gui Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets ${QMMP_LIBRARY} MusicCore)
  endif()
else()
  if(WIN32)
    target_link_libraries(${TARGET_NAME} ${QMMP_LIBRARY} ${QT_QTMULTIMEDIA_LIBRARY} ${QT_QTSCRIPT_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTCORE_LIBRARY} MusicCore)
  else(UNIX)
    target_link_libraries(${TARGET_NAME} ${QMMP_LIBRARY} -lQtMultimediaKit ${QT_QTSCRIPT_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTCORE_LIBRARY} MusicCore)
  endif()
endif()
