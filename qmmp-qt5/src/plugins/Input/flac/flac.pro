include(../../plugins.pri)

HEADERS += decoderflacfactory.h \
           decoder_flac.h \
           cueparser.h \
           flacmetadatamodel.h \
           replaygainreader.h
    
SOURCES += decoder_flac.cpp \
           decoderflacfactory.cpp \
           cueparser.cpp \
           flacmetadatamodel.cpp \
           replaygainreader.cpp
    
unix:android {
    TARGET = $$PLUGINS_PREFIX/../plugin_input_flac
}else{
    TARGET = $$PLUGINS_PREFIX/Input/flac
}

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libflac/include

CONFIG += warn_on \
          plugin \
          link_pkgconfig
          
TEMPLATE = lib

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    unix:android {
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_flac.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libflac.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libflac/lib -lFLAC \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
            -L$$EXTRA_PREFIX/libogg/lib -logg -lqmmp
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libflac/lib -llibFLAC \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll \
            -L$$EXTRA_PREFIX/libogg/lib -logg \
            -lqmmp1 -lm
}
