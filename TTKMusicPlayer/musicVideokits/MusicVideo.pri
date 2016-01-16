#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

INCLUDEPATH += $$PWD

!contains(CONFIG, TTK_MSVC_LIK){
HEADERS  += \
    $$PWD/musicvideocontrol.h \
    $$PWD/musicvideoview.h \
    $$PWD/musicvideotablewidget.h \
    $$PWD/musicvideoplaywidget.h

}

contains(CONFIG, TTK_BUID_LIB){
SOURCES += \
    $$PWD/musicvideocontrol.cpp \
    $$PWD/musicvideoview.cpp \
    $$PWD/musicvideotablewidget.cpp \
    $$PWD/musicvideoplaywidget.cpp


RESOURCES += \
    musicQrc/MusicPlayerVedio.qrc

}
