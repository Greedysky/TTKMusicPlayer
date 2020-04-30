include($$PWD/../../plugins.pri)

HEADERS += monotostereoplugin.h \
           effectmonotostereofactory.h

SOURCES += monotostereoplugin.cpp \
           effectmonotostereofactory.cpp

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
