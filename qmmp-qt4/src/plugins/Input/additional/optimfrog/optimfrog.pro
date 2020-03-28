include($$PWD/../additional.pri)

HEADERS += decoderoptimfrogfactory.h \
           decoder_optimfrog.h \
           optimfrogmetadatamodel.h \
           optimfroghelper.h

SOURCES += decoderoptimfrogfactory.cpp \
           decoder_optimfrog.cpp \
           optimfrogmetadatamodel.cpp \
           optimfroghelper.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = optimfrog

INCLUDEPATH += $$EXTRA_PREFIX/liboptimfrog/include

unix {
    QMAKE_CLEAN = liboptimfrog.so
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/liboptimfrog/lib -lOptimFROG
}
