include(../../plugins.pri)

HEADERS += decoderflacfactory.h \
           decoder_flac.h \
           cueparser.h \
           flacmetadatamodel.h
    
SOURCES += decoder_flac.cpp \
           decoderflacfactory.cpp \
           cueparser.cpp \
           flacmetadatamodel.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/flac
QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libflac.so

INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libflac/include

unix {
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_flac
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_flac.so
        target.path = $$LIB_DIR
    }else{
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libflac/lib -lFLAC \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
            -L$$EXTRA_PREFIX/libogg/lib -logg
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libflac/lib -llibFLAC \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll \
            -L$$EXTRA_PREFIX/libogg/lib -logg \
            -lm
}
