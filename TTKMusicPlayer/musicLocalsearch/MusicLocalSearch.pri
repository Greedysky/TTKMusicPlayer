#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

INCLUDEPATH += $$PWD \
               $$PWD/core

!contains(CONFIG, TTK_MSVC_LIK){
HEADERS  += \
    $$PWD/musiclocalsongsearch.h \
    $$PWD/musiclocalsongsearchedit.h \
    $$PWD/musiclocalsongsearchinlineedit.h \
    $$PWD/musiclocalsongsearchpopwidget.h \
    $$PWD/core/musiclocalsongsearchrecordobject.h

}

contains(CONFIG, TTK_BUID_LIB){
SOURCES += \
    $$PWD/musiclocalsongsearch.cpp \
    $$PWD/musiclocalsongsearchedit.cpp \
    $$PWD/musiclocalsongsearchinlineedit.cpp \
    $$PWD/musiclocalsongsearchpopwidget.cpp \
    $$PWD/core/musiclocalsongsearchrecordobject.cpp

}
