include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Spek/spectrumspek
QMAKE_CLEAN =$$PLUGINS_PREFIX/Spek/libspectrumspek.so

SOURCES += \
        spek-ruler.cc \
        spek-spectrogram.cc \
        spek-audio.cc \
        spek-fft.cc \
        spek-palette.cc \
        spek-pipeline.cc \
        visualspectrumspekfactory.cpp

HEADERS  += \
        spek-audio.h \
        spek-fft.h \
        spek-palette.h \
        spek-pipeline.h \
        spek-ruler.h \
        spek-spectrogram.h \
        visualspectrumspekfactory.h

INCLUDEPATH += $$EXTRA_PREFIX/libav/include

win32:{
    INCLUDEPATH += ./
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec.dll -lavformat.dll -lavutil.dll
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec -lavformat -lavutil
}
