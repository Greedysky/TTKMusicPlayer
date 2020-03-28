include($$PWD/../../plugins.pri)

HEADERS += monotostereoplugin.h \
           effectmonotostereofactory.h

SOURCES += monotostereoplugin.cpp \
           effectmonotostereofactory.cpp

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = monotostereo

unix {
    QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libmonotostereo.so
}
