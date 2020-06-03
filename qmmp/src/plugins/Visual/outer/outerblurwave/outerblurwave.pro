include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += outerblurwave.h \
           visualouterblurwavefactory.h
           
SOURCES += outerblurwave.cpp \
           visualouterblurwavefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
