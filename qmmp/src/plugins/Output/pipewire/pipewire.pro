include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Output

HEADERS += outputpipewirefactory.h \
           outputpipewire.h

SOURCES += outputpipewirefactory.cpp \
           outputpipewire.cpp

INCLUDEPATH += /usr/include/pipewire-0.3 /usr/include/spa-0.2

QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so

LIBS += -lpipewire-0.3
