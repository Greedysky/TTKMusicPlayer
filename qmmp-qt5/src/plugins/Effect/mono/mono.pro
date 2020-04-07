include($$PWD/../../plugins.pri)

HEADERS += monoplugin.h \
           effectmonofactory.h

SOURCES += monoplugin.cpp \
           effectmonofactory.cpp

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
