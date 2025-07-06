QT += core gui

include($$PWD/../../qmmp.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app
DESTDIR = $$OUT_PWD/../../bin/$$TTK_VERSION

LIBS += -L$$DESTDIR -lTTKqmmp

win32:msvc:{
    HEADERS += ../../src/qmmp/volume.h \
               ../../src/qmmp/soundcore.h \
               ../../src/qmmp/qmmpsettings.h \
               ../../src/qmmp/visual.h
}

unix{
    LIBS += -L$$PWD/../../../extra/gcc/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}

TEMPLATE = app

SOURCES += main.cpp \
        mainwindow.cpp \
        musicplaylist.cpp \
        musicplayer.cpp

HEADERS  += mainwindow.h \
        musicplaylist.h \
        musicplayer.h

FORMS    += mainwindow.ui
