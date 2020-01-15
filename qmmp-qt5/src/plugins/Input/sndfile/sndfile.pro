include(../../plugins.pri)

HEADERS += decodersndfilefactory.h \
           decoder_sndfile.h
           
SOURCES += decoder_sndfile.cpp \
           decodersndfilefactory.cpp

TARGET = $$PLUGINS_PREFIX/Input/sndfile

INCLUDEPATH += $$EXTRA_PREFIX/libsndfile/include \
               $$EXTRA_PREFIX/libflac/include \
               $$EXTRA_PREFIX/libvorbis/include \
               $$EXTRA_PREFIX/libogg/include

unix {
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_sndfile
        QMAKE_CLEAN = $$PLUGINS_PREFIX/../libplugin_input_sndfile.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libsndfile.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libsndfile/lib -lsndfile$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libflac/lib -lFLAC$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisenc$$STATIC_LIBRARY_SUFFIX -lvorbis$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libogg/lib -logg$$STATIC_LIBRARY_SUFFIX
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libsndfile/lib -lsndfile \
            -L$$EXTRA_PREFIX/libflac/lib -lflac \
            -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisenc -lvorbis \
            -L$$EXTRA_PREFIX/libogg/lib -logg
}
