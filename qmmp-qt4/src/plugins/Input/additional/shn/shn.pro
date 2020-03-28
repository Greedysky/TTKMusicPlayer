include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

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

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = shn

INCLUDEPATH += $$EXTRA_PREFIX/libshn/include

unix{
    unix:android {
        TARGET = plugin_input_shn
        QMAKE_CLEAN = $$DESTDIR/libplugin_input_shn.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/libshn.so
    }
}
