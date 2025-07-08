include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Light

HEADERS += spek.h \
           spek-audio.h \
           spek-fft.h \
           spek-pipeline.h \
           spek-ruler.h \
           spek-spectrogram.h \
           spek-utils.h \
           lightspectrogramfactory.h

SOURCES += spek-ruler.cc \
           spek-spectrogram.cc \
           spek-audio.cc \
           spek-fft.cc \
           spek-pipeline.cc \
           spek-utils.cc \
           lightspectrogramfactory.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libffmpeg/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libffmpeg/lib -lavcodec.dll -lavformat.dll -lavutil.dll -lswresample.dll
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libffmpeg/lib -lavcodec -lavformat -lavutil -lswresample
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lavcodec -lavformat -lavutil -lswresample
}
