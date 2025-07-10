include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Output

HEADERS += outputpulseaudiofactory.h \
           outputpulseaudio.h

SOURCES += outputpulseaudiofactory.cpp \
           outputpulseaudio.cpp

LIBS += -lpulse

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
