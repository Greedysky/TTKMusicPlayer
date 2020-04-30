#-------------------------------------------------
#
# Project created by QtCreator 2016-01-20T11:12:25
#
#-------------------------------------------------

QT       += core gui

include($$PWD/../../qmmp.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app
DESTDIR = $$OUT_PWD/../../bin/$$TTKMusicPlayer

unix:LIBS += -L$$DESTDIR -lqmmp
win32:LIBS += -L$$DESTDIR -lqmmp1

win32:msvc:{
    HEADERS += ../../src/qmmp/volume.h \
               ../../src/qmmp/soundcore.h \
               ../../src/qmmp/qmmpsettings.h \
               ../../src/qmmp/visual.h
}

unix:{
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
