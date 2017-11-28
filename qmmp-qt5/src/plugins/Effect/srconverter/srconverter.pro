include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Effect/srconverter
QMAKE_CLEAN =$$PLUGINS_PREFIX/Effect/libsrconverter.so

HEADERS += srconverter.h \
           effectsrconverterfactory.h \
           settingsdialog.h

SOURCES += srconverter.cpp \
           effectsrconverterfactory.cpp \
           settingsdialog.cpp

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libsoxr/include \
               $$EXTRA_PREFIX/libsamplerate/include

CONFIG += warn_on \
          plugin

TEMPLATE = lib

FORMS += settingsdialog.ui

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    CONFIG += link_pkgconfig
    LIBS += -L$$EXTRA_PREFIX/libsamplerate/lib -lsamplerate -lqmmp
}

win32 {
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libsamplerate/lib -lsamplerate -lqmmp1
}
