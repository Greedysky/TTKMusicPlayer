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

QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer

win32 {
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -lqmmp0
}
