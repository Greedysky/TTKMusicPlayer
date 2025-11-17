include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodervorbisfactory.h \
           decoder_vorbis.h \
           vorbismetadatamodel.h

SOURCES += decodervorbisfactory.cpp \
           decoder_vorbis.cpp \
           vorbismetadatamodel.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libtaglib/include \
               $$EXTRA_LIB_PATH/libogg/include \
               $$EXTRA_LIB_PATH/libvorbis/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libvorbis/lib -lvorbisfile -lvorbis \
            -L$$EXTRA_LIB_PATH/libogg/lib -logg \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag -lm
    LD_FLAGS += -no-undefined
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libvorbis/lib -lvorbisfile$$STATIC_LIB_SUFFIX -lvorbis$$STATIC_LIB_SUFFIX \
            -L$$EXTRA_LIB_PATH/libogg/lib -logg$$STATIC_LIB_SUFFIX \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lvorbisfile -logg -ltag
}
