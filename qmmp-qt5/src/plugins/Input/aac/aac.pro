include(../../plugins.pri)
FORMS +=
HEADERS += decoderaacfactory.h \
    decoder_aac.h \
    aacfile.h \
    aacmetadatamodel.h
SOURCES += decoder_aac.cpp \
    decoderaacfactory.cpp \
    aacfile.cpp \
    aacmetadatamodel.cpp
TARGET = $$PLUGINS_PREFIX/Input/aac
QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libaac.so
INCLUDEPATH += ../../../
CONFIG += warn_on \
    plugin \
    link_pkgconfig
TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib
LIBS += -lqmmp \
    -lfaad \
    -L/usr/lib \
    -I/usr/include
PKGCONFIG += taglib


isEmpty(LIB_DIR):LIB_DIR = /lib
target.path = $$LIB_DIR/qmmp/Input
INSTALLS += target
