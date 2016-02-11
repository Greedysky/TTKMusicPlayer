#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

TEMPLATE = app
win32{
  TARGET = ../../bin/TTKMusicPlayer
  LIBS += -L../bin -lMusicCore
}
unix{
  TARGET = ../lib/TTKMusicPlayer
  LIBS += -L./lib -lMusicCore
}

contains(CONFIG, TTK_BUILD_LIB){
    CONFIG -= TTK_BUILD_LIB
}

CONFIG += TTK_NO_MSVC_LINK_NEED
win32{
    msvc{
        CONFIG -= TTK_NO_MSVC_LINK_NEED
    }
}

INCLUDEPATH += ../TTKMusicPlayer
!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    ../TTKMusicPlayer/musicapplication.h \
    ../TTKMusicPlayer/musicleftareawidget.h \
    ../TTKMusicPlayer/musictopareawidget.h \
    ../TTKMusicPlayer/musicrightareawidget.h \
    ../TTKMusicPlayer/musicbottomareawidget.h \
    ../TTKMusicPlayer/musicapplicationobject.h

}

include(../TTKMusicPlayer.pri)
unix:VERSION += $$TTKMusicPlayer

SOURCES += \
    musicapplicationmain.cpp

win32{
    RC_FILE = TTKApp.rc
}
