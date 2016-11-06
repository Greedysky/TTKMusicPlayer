include(../../plugins.pri)

HEADERS += decodergmefactory.h \
           decoder_gme.h \
           gmehelper.h
    
SOURCES += decoder_gme.cpp \
           decodergmefactory.cpp \
           gmehelper.cpp

unix:android {
    TARGET = $$PLUGINS_PREFIX/../plugin_input_gme
}else{
    TARGET = $$PLUGINS_PREFIX/Input/gme
}

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libgme/include
               
CONFIG += warn_on \
          plugin
          
TEMPLATE = lib

unix{
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    unix:android {
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_gme.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libgme.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libgme/lib -lgme -lqmmp
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libgme/lib -lgme.dll -lqmmp1
}
