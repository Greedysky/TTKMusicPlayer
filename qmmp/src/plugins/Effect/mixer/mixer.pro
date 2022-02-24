include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

HEADERS += mixerplugin.h \
           effectmixerfactory.h \
           settingsdialog.h

SOURCES += mixerplugin.cpp \
           effectmixerfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
