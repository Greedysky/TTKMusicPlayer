include(../../plugins.pri)

HEADERS += decoderoptimfrogfactory.h \
           decoder_optimfrog.h \
           optimfrogmetadatamodel.h \
           optimfrogwrap.h

SOURCES += decoderoptimfrogfactory.cpp \
           decoder_optimfrog.cpp \
           optimfrogmetadatamodel.cpp \
           optimfrogwrap.cpp

TARGET = $$PLUGINS_PREFIX/Input/optimfrog
QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/liboptimfrog.so

INCLUDEPATH += $$EXTRA_PREFIX/liboptimfrog/include

unix {
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
}

LIBS += -L$$EXTRA_PREFIX/liboptimfrog/lib -lOptimFROG
