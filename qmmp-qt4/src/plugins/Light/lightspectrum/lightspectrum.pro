include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Light
TARGET = lightspectrum

SOURCES += \
        spek-ruler.cc \
        spek-spectrogram.cc \
        spek-audio.cc \
        spek-fft.cc \
        spek-palette.cc \
        spek-pipeline.cc \
        visuallightspectrumfactory.cpp

HEADERS  += \
        spek-audio.h \
        spek-fft.h \
        spek-palette.h \
        spek-pipeline.h \
        spek-ruler.h \
        spek-spectrogram.h \
        visuallightspectrumfactory.h

INCLUDEPATH += $$EXTRA_PREFIX/libav/include

win32:{
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec.dll -lavformat.dll -lavutil.dll
}

unix{
    QMAKE_CLEAN = $$DESTDIR/liblightspectrum.so
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec -lavformat -lavutil
}
