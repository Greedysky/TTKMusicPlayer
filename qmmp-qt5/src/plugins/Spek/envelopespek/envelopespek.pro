include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Spek/envelopespek
QMAKE_CLEAN =$$PLUGINS_PREFIX/Spek/libenvelopespek.so

SOURCES += \
        envelopespek.cpp \
        fft.c \
        visualenvelopespekfactory.cpp

HEADERS  += \
        envelopespek.h \
        fft.h \
        inlines.h \
        visualenvelopespekfactory.h

win32:{
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
