include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decodershnfactory.h \
           decoder_shn.h \
           shnmetadatamodel.h \
           shnhelper.h \
           bitshift.h \
           shndec.h \
           shorten.h
    
SOURCES += decodershnfactory.cpp \
           decoder_shn.cpp \
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

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = shn

unix{
    unix:android {
        TARGET = plugin_input_shn
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libplugin_input_shn.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libshn.so
    }
}
