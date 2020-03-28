include($$PWD/../../plugins.pri)

HEADERS += stereoplugin.h \
           effectstereofactory.h \
           settingsdialog.h

SOURCES += stereoplugin.cpp \
           effectstereofactory.cpp \
           settingsdialog.cpp

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = stereo

FORMS += settingsdialog.ui

unix {
    QMAKE_CLEAN = $$DESTDIR/libstereo.so
    LIBS += -L/usr/lib -I/usr/include
}
