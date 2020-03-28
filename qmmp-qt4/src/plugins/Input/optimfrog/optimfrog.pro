include(../../plugins.pri)

HEADERS += decoderoptimfrogfactory.h \
           decoder_optimfrog.h \
           optimfrogmetadatamodel.h \
           optimfroghelper.h

SOURCES += decoderoptimfrogfactory.cpp \
           decoder_optimfrog.cpp \
           optimfrogmetadatamodel.cpp \
           optimfroghelper.cpp

TARGET = $$PLUGINS_PREFIX/Input/optimfrog

INCLUDEPATH += $$EXTRA_PREFIX/liboptimfrog/include

unix {
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/liboptimfrog.so
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/liboptimfrog/lib -lOptimFROG
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
}
