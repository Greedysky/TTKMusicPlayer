include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += outerrayswave.h \
           visualouterrayswavefactory.h
           
SOURCES += outerrayswave.cpp \
           visualouterrayswavefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
