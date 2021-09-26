include($$PWD/../../plugins.pri)

HEADERS += crystalizerplugin.h \
           effectcrystalizerfactory.h \
           settingsdialog.h

SOURCES += crystalizerplugin.cpp \
           effectcrystalizerfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
