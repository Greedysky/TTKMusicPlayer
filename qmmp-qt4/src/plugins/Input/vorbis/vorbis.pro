include($$PWD/../../plugins.pri)

HEADERS += decodervorbisfactory.h \
           decoder_vorbis.h \
           vorbismetadatamodel.h
    
SOURCES += decodervorbisfactory.cpp \
           decoder_vorbis.cpp \
           vorbismetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = vorbis

INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libogg/include \
               $$EXTRA_PREFIX/libvorbis/include

unix {
    unix:android {
        TARGET = plugin_input_vorbis
        QMAKE_CLEAN = $$DESTDIR/libplugin_input_vorbis.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/libvorbis.so
    }
    LIBS += -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisfile$$STATIC_LIBRARY_SUFFIX -lvorbis$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libogg/lib -logg$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisfile -lvorbis \
            -L$$EXTRA_PREFIX/libogg/lib -logg \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll -lm
    LD_FLAGS += -no-undefined
}
