include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Output
TARGET = $${TARGET}

HEADERS += outputnullfactory.h \
           outputnull.h

SOURCES += outputnullfactory.cpp \
           outputnull.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}