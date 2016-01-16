#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

INCLUDEPATH += $$PWD

!contains(CONFIG, TTK_MSVC_LIK){
HEADERS  += \
    $$PWD/musicremotewidget.h \
    $$PWD/musicremotewidgetforsquare.h \
    $$PWD/musicremotewidgetforrectangle.h \
    $$PWD/musicremotewidgetfordiamond.h \
    $$PWD/musicremotewidgetforcircle.h \
    $$PWD/musicremotewidgetforsimplestyle.h \
    $$PWD/musicremotewidgetforcomplexstyle.h

}

contains(CONFIG, TTK_BUID_LIB){
SOURCES += \
    $$PWD/musicremotewidget.cpp \
    $$PWD/musicremotewidgetforsquare.cpp \
    $$PWD/musicremotewidgetforrectangle.cpp \
    $$PWD/musicremotewidgetfordiamond.cpp \
    $$PWD/musicremotewidgetforcircle.cpp \
    $$PWD/musicremotewidgetforsimplestyle.cpp \
    $$PWD/musicremotewidgetforcomplexstyle.cpp

}
