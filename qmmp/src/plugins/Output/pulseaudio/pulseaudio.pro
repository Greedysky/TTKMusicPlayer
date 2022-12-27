include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Output

HEADERS += outputpulseaudiofactory.h \
           outputpulseaudio.h

SOURCES += outputpulseaudiofactory.cpp \
           outputpulseaudio.cpp

QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so

LIBS += -lpulse
