include(../../plugins.pri)

HEADERS += outputalsa.h \
           outputalsafactory.h  \
           settingsdialog.h
SOURCES += outputalsa.cpp \
           outputalsafactory.cpp  \
           settingsdialog.cpp

TARGET=$$PLUGINS_PREFIX/Output/alsa
QMAKE_CLEAN =$$PLUGINS_PREFIX/Output/libalsa.so

INCLUDEPATH += ../../../
QMAKE_LIBDIR += ../../../../lib

CONFIG += warn_on \
thread \
plugin \
link_pkgconfig

TEMPLATE = lib
LIBS += -lqmmp
PKGCONFIG += alsa

FORMS += settingsdialog.ui



isEmpty (LIB_DIR){
LIB_DIR = /lib
}

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
