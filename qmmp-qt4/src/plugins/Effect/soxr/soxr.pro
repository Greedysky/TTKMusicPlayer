include(../../plugins.pri)

HEADERS += soxresampler.h \
           effectsoxrfactory.h \
           settingsdialog.h

SOURCES += soxresampler.cpp \
           effectsoxrfactory.cpp \
           settingsdialog.cpp

TARGET=$$PLUGINS_PREFIX/Effect/soxr
QMAKE_CLEAN =$$PLUGINS_PREFIX/Effect/libsoxr.so

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libsoxr/include \
               
CONFIG += warn_on \
          plugin

TEMPLATE = lib

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    CONFIG += link_pkgconfig
    LIBS += -lqmmp -L/usr/lib -I/usr/include
}

win32 {
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libsoxr/lib -lsoxr -lqmmp0
}

FORMS += settingsdialog.ui
