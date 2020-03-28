include($$PWD/../../plugins.pri)

HEADERS += monoplugin.h \
           effectmonofactory.h

SOURCES += monoplugin.cpp \
           effectmonofactory.cpp

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = mono

unix {
    QMAKE_CLEAN = $$DESTDIR/libmono.so
}
