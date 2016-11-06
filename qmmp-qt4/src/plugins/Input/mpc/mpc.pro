include(../../plugins.pri)

HEADERS += decodermpcfactory.h \
           decoder_mpc.h \
           mpcmetadatamodel.h
    
SOURCES += decoder_mpc.cpp \
           decodermpcfactory.cpp \
           mpcmetadatamodel.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/mpc

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
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    DEFINES += MPC_OLD_API
    LIBS += -L$$EXTRA_PREFIX/libmpcdec/lib -lmpcdec \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag -lqmmp
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libmpc.so
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libmpcdec/lib -lmpcdec \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll -lqmmp0
    #DEFINES += MPC_OLD_API
}
