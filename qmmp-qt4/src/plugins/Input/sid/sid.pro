include(../../plugins.pri)

HEADERS += decodersidfactory.h \
           decoder_sid.h \
           sidhelper.h
    
SOURCES += decoder_sid.cpp \
           decodersidfactory.cpp \
           sidhelper.cpp

TARGET = $$PLUGINS_PREFIX/Input/sid
QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libsid.so

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libsidplayfp/include
            
CONFIG += warn_on \
          plugin \
          link_pkgconfig
          
TEMPLATE = lib

unix{
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libsidplayfp/lib -lsidplayfp -lqmmp
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libsidplayfp/lib -lsidplayfp -lqmmp0
}
