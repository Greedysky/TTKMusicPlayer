include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

HEADERS += mufflerplugin.h \
           effectmufflerfactory.h

SOURCES += mufflerplugin.cpp \
           effectmufflerfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
