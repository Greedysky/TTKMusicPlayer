include($$PWD/../../plugins.pri)

HEADERS += outputnullfactory.h \
           outputnull.h

SOURCES += outputnullfactory.cpp \
           outputnull.cpp

DESTDIR = $$PLUGINS_PREFIX/Output
TARGET = $${TARGET}

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
