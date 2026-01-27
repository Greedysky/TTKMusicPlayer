include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Output

HEADERS += outputpipewirefactory.h \
           outputpipewire.h

SOURCES += outputpipewirefactory.cpp \
           outputpipewire.cpp

PKGCONFIG += libpipewire-0.3 libspa-0.2

QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
