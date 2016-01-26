include(../../plugins.pri)
FORMS += settingsdialog.ui
HEADERS += decoderwildmidifactory.h \
    decoder_wildmidi.h \
    wildmidihelper.h \
    settingsdialog.h
SOURCES += decoder_wildmidi.cpp \
    decoderwildmidifactory.cpp \
    wildmidihelper.cpp \
    settingsdialog.cpp
TARGET = $$PLUGINS_PREFIX/Input/wildmidi
QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libwildmidi.so
INCLUDEPATH += ../../../
CONFIG += warn_on \
    plugin
TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib
LIBS += -lqmmp \
    -L/usr/lib \
    -I/usr/include \
    -lWildMidi


isEmpty (LIB_DIR):LIB_DIR = /lib
target.path = $$LIB_DIR/qmmp/Input
INSTALLS += target
