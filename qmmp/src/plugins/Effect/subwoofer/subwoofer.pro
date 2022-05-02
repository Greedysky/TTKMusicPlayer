include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

HEADERS += subwooferplugin.h \
           effectsubwooferfactory.h \
           settingsdialog.h

SOURCES += subwooferplugin.cpp \
           effectsubwooferfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
