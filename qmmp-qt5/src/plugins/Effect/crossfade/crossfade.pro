include($$PWD/../../plugins.pri)

HEADERS += crossfadeplugin.h \
           effectcrossfadefactory.h \
           settingsdialog.h

SOURCES += crossfadeplugin.cpp \
           effectcrossfadefactory.cpp \
           settingsdialog.cpp

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = crossfade

FORMS += settingsdialog.ui

unix {
    QMAKE_CLEAN = libcrossfade.so
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
