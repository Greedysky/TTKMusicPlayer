include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += ethereality.h \
           flowethereality.h \
           visualflowetherealityfactory.h
           
SOURCES += ethereality.cpp \
           flowethereality.cpp \
           visualflowetherealityfactory.cpp

RESOURCES += flowethereality.qrc

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
