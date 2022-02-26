include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

HEADERS += mufflerplugin.h \
           effecmufflerfactory.h

SOURCES += mufflerplugin.cpp \
           effecmufflerfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
