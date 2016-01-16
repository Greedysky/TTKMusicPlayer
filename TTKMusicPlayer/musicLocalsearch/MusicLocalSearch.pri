#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

INCLUDEPATH += $$PWD \
               $$PWD/core

!contains(CONFIG, TTK_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musiclocalsongsearch.h \
    $$PWD/musiclocalsongsearchedit.h \
    $$PWD/musiclocalsongsearchinlineedit.h \
    $$PWD/musiclocalsongsearchpopwidget.h \
    $$PWD/core/musiclocalsongsearchrecordobject.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musiclocalsongsearch.cpp \
    $$PWD/musiclocalsongsearchedit.cpp \
    $$PWD/musiclocalsongsearchinlineedit.cpp \
    $$PWD/musiclocalsongsearchpopwidget.cpp \
    $$PWD/core/musiclocalsongsearchrecordobject.cpp

}
