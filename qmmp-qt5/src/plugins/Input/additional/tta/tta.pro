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
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libplugin_input_tta.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libtta.so
    }
}
