include(../../plugins.pri)

HEADERS += stereoplugin.h \
           effectstereofactory.h \
           settingsdialog.h

SOURCES += stereoplugin.cpp \
           effectstereofactory.cpp \
           settingsdialog.cpp

TARGET =$$PLUGINS_PREFIX/Effect/stereo

INCLUDEPATH += ../../../

CONFIG += warn_on \
          plugin

TEMPLATE = lib

FORMS += settingsdialog.ui

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target

    LIBS += -lqmmp -L/usr/lib -I/usr/include
    QMAKE_CLEAN =$$PLUGINS_PREFIX/Effect/libstereo.so
}

win32 {
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -lqmmp1
}

