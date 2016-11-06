include(../../plugins.pri)

HEADERS += decodermpcfactory.h \
           decoder_mpc.h \
           mpcmetadatamodel.h
    
SOURCES += decoder_mpc.cpp \
           decodermpcfactory.cpp \
           mpcmetadatamodel.cpp

unix:android {
    TARGET = $$PLUGINS_PREFIX/../plugin_input_mpc
}else{
    TARGET = $$PLUGINS_PREFIX/Input/mpc
}

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libmpcdec/include
                   
CONFIG += warn_on \
          plugin \
          link_pkgconfig
    
TEMPLATE = lib

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    unix:android {
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_mpc.so
        target.path = $$LIB_DIR
    }else{
        DEFINES += MPC_OLD_API
        QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libmpc.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libmpcdec/lib -lmpcdec \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag -lqmmp
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libmpcdec/lib -lmpcdec \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll -lqmmp1
    #DEFINES += MPC_OLD_API
}
