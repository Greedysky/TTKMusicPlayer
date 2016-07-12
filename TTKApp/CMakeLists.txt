cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKMusicPlayer)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_APP_SOURCES
  musicapplicationmain.cpp
)

add_executable(${TARGET_NAME} TTKApp.rc ${MUSIC_APP_SOURCES})
add_dependencies(${TARGET_NAME} MusicCore)

if(TTK_QT_VERSION VERSION_GREATER "4")
  list(APPEND QT5_LIBS Qt5::Core Qt5::Network Qt5::Sql Qt5::Xml Qt5::Gui Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets Qt5::WinExtras ${QMMP_LIBRARY} MusicExtras zlib MusicCore -lIphlpapi -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT5_LIBS Qt5::WinExtras -lIphlpapi -luser32)
    list(APPEND QT5_LIBS Qt5::X11Extras)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT5_LIBS})
else()
  list(APPEND QT4_LIBS ${QMMP_LIBRARY} ${QT_QTMULTIMEDIA_LIBRARY} ${QT_QTSCRIPT_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTCORE_LIBRARY} MusicExtras zlib MusicCore -lIphlpapi -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT4_LIBS ${QT_QTMULTIMEDIA_LIBRARY} -lIphlpapi -luser32)
    list(APPEND QT4_LIBS -lQtMultimediaKit)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT4_LIBS})
endif()
