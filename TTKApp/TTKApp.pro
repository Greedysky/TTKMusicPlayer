#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------


TARGET = ../../bin/TTKMusicPlayer
TEMPLATE = app
LIBS += -L../bin -lMusicCore

contains(CONFIG, TTK_BUILD_LIB){
    CONFIG -= TTK_BUILD_LIB
}
win32{
    CONFIG += TTK_MSVC_LINK_NEED
    msvc{
        CONFIG -= TTK_MSVC_LINK_NEED
    }
}

INCLUDEPATH += ../TTKMusicPlayer
!contains(CONFIG, TTK_MSVC_LINK_NEED){
HEADERS  += \
    ../TTKMusicPlayer/musicapplication.h \
    ../TTKMusicPlayer/musicleftareawidget.h \
    ../TTKMusicPlayer/musictopareawidget.h \
    ../TTKMusicPlayer/musicrightareawidget.h \
    ../TTKMusicPlayer/musicbottomareawidget.h \
    ../TTKMusicPlayer/musicapplicationobject.h

}

include(../TTKMusicPlayer.pri)

SOURCES += \
    musicapplicationmain.cpp

win32{
    RC_FILE = TTKApp.rc
}
