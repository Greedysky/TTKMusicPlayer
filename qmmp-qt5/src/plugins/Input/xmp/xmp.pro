include(../../plugins.pri)

HEADERS += decoderxmpfactory.h \
           decoder_xmp.h \
           xmpmetadatamodel.h

SOURCES += decoder_xmp.cpp \
           decoderxmpfactory.cpp \
           xmpmetadatamodel.cpp


TARGET = $$PLUGINS_PREFIX/Input/xmp
QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libxmp.so

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libxmp/include

CONFIG += warn_on \
          plugin

TEMPLATE = lib

unix:{
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayerb
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libxmp/lib -lxmp -lqmmp
}

win32:{
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libxmp/lib -lxmp_dll -lqmmp1
}
