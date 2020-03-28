include(../../plugins.pri)
include(../common/common.pri)

HEADERS += decodershnfactory.h \
           decoder_shn.h \
           shnmetadatamodel.h \
           shnhelper.h \
           bitshift.h \
           shndec.h \
           shorten.h
    
SOURCES += decoder_shn.cpp \
           decodershnfactory.cpp \
           shnmetadatamodel.cpp \
           shnhelper.cpp \
           array.c \
           convert.c \
           misc.c \
           output.c \
           seek.c \
           shndec.c \
           shorten.c \
           sulawalaw.c \
           vario.c \
           wave.c

TARGET = $$PLUGINS_PREFIX/Input/shn

INCLUDEPATH += $$EXTRA_PREFIX/libshn/include

unix{
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_shn
        QMAKE_CLEAN = $$PLUGINS_PREFIX/../libplugin_input_shn.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libshn.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
}
