include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += ethereality.h \
           floridethereality.h \
           visualfloridetherealityfactory.h
           
SOURCES += ethereality.cpp \
           floridethereality.cpp \
           visualfloridetherealityfactory.cpp

RESOURCES += floridethereality.qrc

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
