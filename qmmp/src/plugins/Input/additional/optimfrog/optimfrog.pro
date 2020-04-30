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
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/liboptimfrog/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/liboptimfrog/lib -lOptimFROG
}
