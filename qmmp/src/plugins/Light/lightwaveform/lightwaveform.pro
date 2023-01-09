include($$PWD/../../plugins.pri)
include($$PWD/../../Visual/common/widget.pri)

DESTDIR = $$PLUGINS_PREFIX/Light

SOURCES += lightwaveform.cpp \
           lightwaveformfactory.cpp

HEADERS += lightwaveform.h \
           lightwaveformfactory.h

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
