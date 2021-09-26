include($$PWD/../../plugins.pri)

HEADERS += echoplugin.h \
           effectechofactory.h \
           settingsdialog.h

SOURCES += echoplugin.cpp \
           effectechofactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
