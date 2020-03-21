include(../../plugins.pri)

HEADERS += decoderspeexfactory.h \
           decoder_speex.h \
           speexhelper.h \
           speexmetadatamodel.h
    
SOURCES += decoderspeexfactory.cpp \
           decoder_speex.cpp \
           speexhelper.cpp \
           speexmetadatamodel.cpp

TARGET = $$PLUGINS_PREFIX/Input/speex

INCLUDEPATH += $$EXTRA_PREFIX/libspeex/include \
               $$EXTRA_PREFIX/libogg/include

unix{
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_speex
        QMAKE_CLEAN = $$PLUGINS_PREFIX/../libplugin_input_speex.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libspeex.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libspeex/lib -lspeex$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libogg/lib -logg$$STATIC_LIBRARY_SUFFIX
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libspeex/lib -lspeex.dll \
            -L$$EXTRA_PREFIX/libogg/lib -logg
}
