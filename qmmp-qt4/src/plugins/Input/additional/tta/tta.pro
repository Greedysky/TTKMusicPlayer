include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decoderttafactory.h \
           ttametadatamodel.h \
           decoder_tta.h \
           ttahelper.h \
           filter.h \
           ttadec.h
    
SOURCES += decoderttafactory.cpp \
           ttametadatamodel.cpp \
           decoder_tta.cpp \
           ttahelper.cpp \
           ttadec.c

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = tta

unix{
    unix:android {
        TARGET = plugin_input_tta
        QMAKE_CLEAN = $$DESTDIR/libplugin_input_tta.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/libtta.so
    }
}
