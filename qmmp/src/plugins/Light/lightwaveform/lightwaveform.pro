include($$PWD/../../plugins.pri)
include($$PWD/../../Visual/common/widget.pri)

DESTDIR = $$PLUGINS_PREFIX/Light

HEADERS += lightwaveform.h \
           lightwaveformfactory.h

SOURCES += lightwaveform.cpp \
           lightwaveformfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
