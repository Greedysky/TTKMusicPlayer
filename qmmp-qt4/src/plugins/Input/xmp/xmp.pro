include(../../plugins.pri)

HEADERS += decoderxmpfactory.h \
           decoder_xmp.h \
           xmpmetadatamodel.h

SOURCES += decoder_xmp.cpp \
           decoderxmpfactory.cpp \
           xmpmetadatamodel.cpp

TARGET = $$PLUGINS_PREFIX/Input/xmp
QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libxmp.so

INCLUDEPATH += $$EXTRA_PREFIX/libxmp/include

unix:{
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libxmp/lib -lxmp
}

win32:{
    LIBS += -L$$EXTRA_PREFIX/libxmp/lib -lxmp_dll
}
