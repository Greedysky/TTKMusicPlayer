include(../../plugins.pri)

HEADERS += decodervorbisfactory.h \
           decoder_vorbis.h \
           vorbismetadatamodel.h
    
SOURCES += decoder_vorbis.cpp \
           decodervorbisfactory.cpp \
           vorbismetadatamodel.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/vorbis
QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libvorbis.so

INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libogg/include \
               $$EXTRA_PREFIX/libvorbis/include

unix {
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_vorbis
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_vorbis.so
        target.path = $$LIB_DIR
    }else{
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisfile -lvorbis \
            -L$$EXTRA_PREFIX/libogg/lib -logg \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisfile -lvorbis \
            -L$$EXTRA_PREFIX/libogg/lib -logg \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll -lm
    LD_FLAGS += -no-undefined
}
