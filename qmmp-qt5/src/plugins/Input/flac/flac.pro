include($$PWD/../../plugins.pri)

HEADERS += decoderflacfactory.h \
           decoder_flac.h \
           cueparser.h \
           flacmetadatamodel.h
    
SOURCES += decoder_flac.cpp \
           decoderflacfactory.cpp \
           cueparser.cpp \
           flacmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = flac

INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libflac/include

unix {
    unix:android {
        TARGET = plugin_input_flac
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libplugin_input_flac.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libflac.so
    }
    LIBS += -L$$EXTRA_PREFIX/libflac/lib -lFLAC$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libogg/lib -logg$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libflac/lib -llibFLAC \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll \
            -L$$EXTRA_PREFIX/libogg/lib -logg \
            -lm
}
