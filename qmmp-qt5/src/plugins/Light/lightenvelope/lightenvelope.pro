include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Light/lightenvelope
QMAKE_CLEAN =$$PLUGINS_PREFIX/Light/liblightenvelopek.so

SOURCES += \
        lightenvelope.cpp \
        fft.c \
        visuallightenvelopefactory.cpp

HEADERS  += \
        lightenvelope.h \
        fft.h \
        inlines.h \
        visuallightenvelopefactory.h

win32:{
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
