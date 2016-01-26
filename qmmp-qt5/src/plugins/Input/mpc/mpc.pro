include(../../plugins.pri)
FORMS +=
HEADERS += decodermpcfactory.h \
    decoder_mpc.h \
    mpcmetadatamodel.h
SOURCES += decoder_mpc.cpp \
    decodermpcfactory.cpp \
    mpcmetadatamodel.cpp
TARGET = $$PLUGINS_PREFIX/Input/mpc

INCLUDEPATH += ../../../
CONFIG += warn_on \
    plugin \
    link_pkgconfig
TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib


unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target

    PKGCONFIG += taglib
    LIBS += -lqmmp -lmpcdec  -I/usr/include
    exists(/usr/include/mpcdec/mpcdec.h):DEFINES += MPC_OLD_API
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libmpc.so
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin

    gcc{
        INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
                       $$EXTRA_PREFIX/libmpcdec/include
        LIBS += -L$$EXTRA_PREFIX/libmpcdec/lib -lmpcdec \
                -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll \
                -lqmmp1
    }
#    LIBS += -lqmmp0 -lmpcdec -ltag.dll
    #DEFINES += MPC_OLD_API
}
