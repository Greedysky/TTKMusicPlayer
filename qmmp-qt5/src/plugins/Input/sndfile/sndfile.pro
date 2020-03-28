include($$PWD/../../plugins.pri)

HEADERS += decodersndfilefactory.h \
           decoder_sndfile.h
           
SOURCES += decoder_sndfile.cpp \
           decodersndfilefactory.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = sndfile

INCLUDEPATH += $$EXTRA_PREFIX/libsndfile/include \
               $$EXTRA_PREFIX/libflac/include \
               $$EXTRA_PREFIX/libvorbis/include \
               $$EXTRA_PREFIX/libogg/include

unix {
    unix:android {
        TARGET = plugin_input_sndfile
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libplugin_input_sndfile.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libsndfile.so
    }
    LIBS += -L$$EXTRA_PREFIX/libsndfile/lib -lsndfile$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libflac/lib -lFLAC$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisenc$$STATIC_LIBRARY_SUFFIX -lvorbis$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libogg/lib -logg$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libsndfile/lib -lsndfile \
            -L$$EXTRA_PREFIX/libflac/lib -lflac \
            -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisenc -lvorbis \
            -L$$EXTRA_PREFIX/libogg/lib -logg
}
