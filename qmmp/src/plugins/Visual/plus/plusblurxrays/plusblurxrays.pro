include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += plusblurxrays.h \
           visualplusblurxraysfactory.h
           
SOURCES += plusblurxrays.cpp \
           visualplusblurxraysfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
