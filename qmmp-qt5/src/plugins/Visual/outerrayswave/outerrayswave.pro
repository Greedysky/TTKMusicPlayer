include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = outerrayswave

HEADERS += outerrayswave.h \
           visualouterrayswavefactory.h
           
SOURCES += outerrayswave.cpp \
           visualouterrayswavefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libouterrayswave.so
}
