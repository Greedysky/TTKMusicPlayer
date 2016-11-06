include(../../plugins.pri)

HEADERS += decodermodplugfactory.h \
           decoder_modplug.h \
           archivereader.h \
           modplugmetadatamodel.h
    
SOURCES += decoder_modplug.cpp \
           decodermodplugfactory.cpp \
           archivereader.cpp \
           modplugmetadatamodel.cpp
    
unix:android {
    TARGET = $$PLUGINS_PREFIX/../plugin_input_modplug
}else{
    TARGET = $$PLUGINS_PREFIX/Input/modplug
}

DEFINES += HAVE_STDINT_H \
           HAVE_INTTYPES_H
           
INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libmodplug/include

CONFIG += warn_on \
          plugin \
          link_pkgconfig
    
TEMPLATE = lib

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    unix:android {
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_modplug.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libmodplug.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libmodplug/lib -lmodplug -lqmmp
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libmodplug/lib -lmodplug -lqmmp1
    DEFINES -= UNICODE
}
