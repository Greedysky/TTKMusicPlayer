include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

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

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = tta

INCLUDEPATH += $$EXTRA_PREFIX/libtta/include

unix{
    unix:android {
        TARGET = plugin_input_tta
        QMAKE_CLEAN = libplugin_input_tta.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = libtta.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
}
