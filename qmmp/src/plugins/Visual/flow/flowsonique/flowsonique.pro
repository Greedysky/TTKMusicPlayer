include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += soniquewidget.h \
           soniqueplugin.h \
           visual.h \
           kiss_fft.h \
           visualflowsoniquefactory.h

SOURCES += soniquewidget.cpp \
           soniqueplugin.cpp \
           kiss_fft.c \
           visualflowsoniquefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
