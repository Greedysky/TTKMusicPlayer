include(../../plugins.pri)

HEADERS += decodersndfilefactory.h \
           decoder_sndfile.h
           
SOURCES += decoder_sndfile.cpp \
           decodersndfilefactory.cpp

unix:android {
    TARGET = $$PLUGINS_PREFIX/../plugin_input_sndfile
}else{
    TARGET = $$PLUGINS_PREFIX/Input/sndfile
}

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libsndfile/include \
               $$EXTRA_PREFIX/libflac/include \
               $$EXTRA_PREFIX/libvorbis/include \
               $$EXTRA_PREFIX/libogg/include
                   
CONFIG += warn_on \
          plugin \
          link_pkgconfig

TEMPLATE = lib


unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    unix:android {
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_sndfile.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libsndfile.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libsndfile/lib -lsndfile \
            -L$$EXTRA_PREFIX/libflac/lib -lFLAC \
            -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisenc -lvorbis \
            -L$$EXTRA_PREFIX/libogg/lib -logg \
            -lqmmp
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libsndfile/lib -lsndfile \
            -L$$EXTRA_PREFIX/libflac/lib -lflac \
            -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisenc -lvorbis \
            -L$$EXTRA_PREFIX/libogg/lib -logg -lqmmp1
}
