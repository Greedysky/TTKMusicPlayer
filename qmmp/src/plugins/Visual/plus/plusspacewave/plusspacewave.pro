include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += plusspacewave.h \
           visualplusspacewavefactory.h

SOURCES += plusspacewave.cpp \
           visualplusspacewavefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
