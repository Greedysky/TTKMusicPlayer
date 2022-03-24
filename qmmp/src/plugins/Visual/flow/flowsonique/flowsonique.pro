include($$PWD/../../Visual.pri)

QT += opengl
DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += soniquewidget.h \
           visual.h \
           kiss_fft.h \
           visualflowsoniquefactory.h

SOURCES += soniquewidget.cpp \
           kiss_fft.c \
           visualflowsoniquefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
