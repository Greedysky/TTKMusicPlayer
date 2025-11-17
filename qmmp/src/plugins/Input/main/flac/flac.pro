include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderflacfactory.h \
           decoder_flac.h \
           flacmetadatamodel.h

SOURCES += decoderflacfactory.cpp \
           decoder_flac.cpp \
           flacmetadatamodel.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libtaglib/include \
               $$EXTRA_LIB_PATH/libflac/include

DEFINES += FLAC__NO_DLL

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libflac/lib -lFLAC \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag \
            -L$$EXTRA_LIB_PATH/libogg/lib -logg \
            -lm
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libflac/lib -lFLAC$$STATIC_LIB_SUFFIX \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag$$STATIC_LIB_SUFFIX \
            -L$$EXTRA_LIB_PATH/libogg/lib -logg$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lFLAC -ltag -logg
}
