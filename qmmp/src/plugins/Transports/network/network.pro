include($$PWD/../../plugins.pri)

QT += network
DESTDIR = $$PLUGINS_PREFIX/Transports
TARGET = $${TARGET}

HEADERS += networkinputfactory.h \
           networkinputsource.h \
           networkstreamreader.h

SOURCES += networkinputfactory.cpp \
           networkinputsource.cpp \
           networkstreamreader.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
