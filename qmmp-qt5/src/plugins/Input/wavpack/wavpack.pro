include($$PWD/../../plugins.pri)

HEADERS += decoderwavpackfactory.h \
           decoder_wavpack.h \
           cueparser.h \
           wavpackmetadatamodel.h
    
SOURCES += decoderwavpackfactory.cpp \
           decoder_wavpack.cpp \
           cueparser.cpp \
           wavpackmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libwavpack/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libwavpack/lib -lwavpack$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libwavpack/lib -lwavpack
}
