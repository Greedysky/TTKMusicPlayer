include(../../plugins.pri)

HEADERS += decoderopusfactory.h \
           decoder_opus.h \
           opusmetadatamodel.h \
           replaygainreader.h
    
SOURCES += decoder_opus.cpp \
           decoderopusfactory.cpp \
           opusmetadatamodel.cpp \
           replaygainreader.cpp
    
unix:android {
    TARGET = $$PLUGINS_PREFIX/../plugin_input_opus
}else{
    TARGET = $$PLUGINS_PREFIX/Input/opus
}

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libopusfile/include \
               $$EXTRA_PREFIX/libogg/include \
               $$EXTRA_PREFIX/libtaglib/include
               
CONFIG += warn_on \
          plugin \
          link_pkgconfig
          
TEMPLATE = lib

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    unix:android {
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_opus.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libopus.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libopusfile/lib -lopusfile -lopus \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
            -L$$EXTRA_PREFIX/libogg/lib -logg -lqmmp
}


win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libopusfile/lib -lopusfile -lopus \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll -lqmmp1 -lm
}
