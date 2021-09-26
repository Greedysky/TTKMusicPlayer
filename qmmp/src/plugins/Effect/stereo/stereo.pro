include($$PWD/../../plugins.pri)

HEADERS += stereoplugin.h \
           effectstereofactory.h \
           settingsdialog.h

SOURCES += stereoplugin.cpp \
           effectstereofactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L/usr/lib -I/usr/include
}
