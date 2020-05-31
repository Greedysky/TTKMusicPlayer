include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += outerewave.h \
           visualouterewavefactory.h
           
SOURCES += outerewave.cpp \
           visualouterewavefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
