include($$PWD/../../plugins.pri)

QT += opengl
DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += soniqueplugin.h \
           soniquewidget.h \
           vis.h \
           kiss_fft.h \
           visualflowsoniquefactory.h

SOURCES += soniqueplugin.cpp \
           soniquewidget.cpp \
           kiss_fft.c \
           visualflowsoniquefactory.cpp

# Disbale gcc warnings
QMAKE_CFLAGS += -w
QMAKE_CXXFLAGS += -w

win32:{
    LIBS += -lglu32 -lopengl32
}