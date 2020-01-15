include(../../plugins.pri)

HEADERS += decoderopusfactory.h \
           decoder_opus.h \
           opusmetadatamodel.h
    
SOURCES += decoder_opus.cpp \
           decoderopusfactory.cpp \
           opusmetadatamodel.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/opus

INCLUDEPATH += $$EXTRA_PREFIX/libopusfile/include \
               $$EXTRA_PREFIX/libogg/include \
               $$EXTRA_PREFIX/libtaglib/include

unix {
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_opus
        QMAKE_CLEAN = $$PLUGINS_PREFIX/../libplugin_input_opus.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libopus.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libopusfile/lib -lopusfile$$STATIC_LIBRARY_SUFFIX -lopus$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libogg/lib -logg$$STATIC_LIBRARY_SUFFIX
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libopusfile/lib -lopusfile -lopus \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll -lm
}
