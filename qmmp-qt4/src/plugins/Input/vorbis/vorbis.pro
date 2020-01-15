include(../../plugins.pri)

HEADERS += decodervorbisfactory.h \
           decoder_vorbis.h \
           vorbismetadatamodel.h
    
SOURCES += decoder_vorbis.cpp \
           decodervorbisfactory.cpp \
           vorbismetadatamodel.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/vorbis

INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libogg/include \
               $$EXTRA_PREFIX/libvorbis/include

unix {
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_vorbis
        QMAKE_CLEAN = $$PLUGINS_PREFIX/../libplugin_input_vorbis.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libvorbis.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisfile$$STATIC_LIBRARY_SUFFIX -lvorbis$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libogg/lib -logg$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisfile -lvorbis \
            -L$$EXTRA_PREFIX/libogg/lib -logg \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll -lm
    LD_FLAGS += -no-undefined
}
