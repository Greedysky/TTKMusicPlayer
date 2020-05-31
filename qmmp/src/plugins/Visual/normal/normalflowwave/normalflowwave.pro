include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += normalflowwave.h \
           visualnormalflowwavefactory.h

SOURCES += normalflowwave.cpp \
           visualnormalflowwavefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
