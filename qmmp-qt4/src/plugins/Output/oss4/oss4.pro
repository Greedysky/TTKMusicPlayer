include(../../plugins.pri)
FORMS += settingsdialog.ui

HEADERS += outputoss4factory.h \
           outputoss4.h \
           settingsdialog.h


SOURCES += outputoss4factory.cpp \
           outputoss4.cpp \
           settingsdialog.cpp

TARGET=$$PLUGINS_PREFIX/Output/oss4
QMAKE_CLEAN =$$PLUGINS_PREFIX/Output/liboss4.so


INCLUDEPATH += ../../../
QMAKE_LIBDIR += ../../../../lib
CONFIG += warn_on \
thread \
plugin

DEFINES += HAVE_SYS_SOUNDCARD_H
INCLUDEPATH += /usr/lib/oss/include

TEMPLATE = lib
LIBS += -lqmmp

isEmpty (LIB_DIR){
LIB_DIR = /lib
}

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
