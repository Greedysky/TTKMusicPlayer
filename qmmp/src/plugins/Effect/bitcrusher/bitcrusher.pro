include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

HEADERS += bitcrusherplugin.h \
           effectbitcrusherfactory.h \
           settingsdialog.h

SOURCES += bitcrusherplugin.cpp \
           effectbitcrusherfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
