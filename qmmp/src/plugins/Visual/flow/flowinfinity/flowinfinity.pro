include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += infinitywidget.h \
           visualflowinfinityfactory.h

SOURCES += infinitywidget.cpp \
           visualflowinfinityfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
