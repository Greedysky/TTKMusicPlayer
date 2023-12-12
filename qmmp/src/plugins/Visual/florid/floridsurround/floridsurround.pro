include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += floridsurround.h \
           visualfloridsurroundfactory.h

SOURCES += floridsurround.cpp \
           visualfloridsurroundfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
