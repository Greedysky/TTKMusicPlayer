include(../../plugins.pri)
include(../common/common.pri)

HEADERS += decoderttafactory.h \
           ttametadatamodel.h \
           decoder_tta.h \
           ttahelper.h \
           filter.h \
           ttadec.h
    
SOURCES += decoder_tta.cpp \
           ttametadatamodel.cpp \
           decoderttafactory.cpp \
           ttahelper.cpp \
           ttadec.c

TARGET = $$PLUGINS_PREFIX/Input/tta

INCLUDEPATH += $$EXTRA_PREFIX/libtta/include

unix{
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_tta
        QMAKE_CLEAN = $$PLUGINS_PREFIX/../libplugin_input_tta.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libtta.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
}
