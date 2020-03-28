#-------------------------------------------------
#
# Project created by QtCreator 2016-01-20T11:12:25
#
#-------------------------------------------------

QT       += core gui

include($$PWD/../../qmmp.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app
win32:DESTDIR = $$OUT_PWD/../../bin/$$TTKMusicPlayer
unix:DESTDIR = $$OUT_PWD/../../lib/$$TTKMusicPlayer

win32:{
    LIBS += -L$$OUT_PWD/../../bin/$$TTKMusicPlayer -lqmmp0
}
unix:{
    LIBS += -L$$OUT_PWD/../../lib/$$TTKMusicPlayer -lqmmp
    LIBS += -L$$PWD/../../../extra/gcc/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        musicplaylist.cpp \
        musicplayer.cpp

HEADERS  += mainwindow.h \
        musicplaylist.h \
        musicplayer.h

FORMS    += mainwindow.ui
