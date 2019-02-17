include(../../plugins.pri)

HEADERS += decoderopusfactory.h \
           decoder_opus.h \
           opusmetadatamodel.h
    
SOURCES += decoder_opus.cpp \
           decoderopusfactory.cpp \
           opusmetadatamodel.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/opus
QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libopus.so

INCLUDEPATH += $$EXTRA_PREFIX/libopusfile/include \
               $$EXTRA_PREFIX/libogg/include \
               $$EXTRA_PREFIX/libtaglib/include

unix {
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_opus
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_opus.so
        target.path = $$LIB_DIR
    }else{
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libopusfile/lib -lopusfile -lopus \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
            -L$$EXTRA_PREFIX/libogg/lib -logg
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libopusfile/lib -lopusfile -lopus \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll -lm
}
