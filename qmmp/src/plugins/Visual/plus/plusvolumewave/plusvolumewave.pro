include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += plusvolumewave.h \
           visualplusvolumewavefactory.h
           
SOURCES += plusvolumewave.cpp \
           visualplusvolumewavefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
