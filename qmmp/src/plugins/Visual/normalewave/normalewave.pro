include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += normalewave.h \
           visualnormalewavefactory.h
           
SOURCES += normalewave.cpp \
           visualnormalewavefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
