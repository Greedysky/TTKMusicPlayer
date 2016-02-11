cmake_minimum_required(VERSION 2.8.11)

project(TTKMusicPlayer)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_THREAD)

set(MUSIC_APP_SOURCES
  musicapplicationmain.cpp
)

ADD_EXECUTABLE(TTKMusicPlayer TTKApp.rc ${MUSIC_APP_SOURCES})
add_dependencies(TTKMusicPlayer MusicCore)
target_link_libraries(TTKMusicPlayer Qt5::Core Qt5::Network Qt5::Sql Qt5::Xml Qt5::Gui Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets Qt5::WinExtras ${QMMP_LIBRARY} MusicCore)