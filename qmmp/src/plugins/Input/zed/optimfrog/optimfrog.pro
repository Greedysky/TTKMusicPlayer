include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderoptimfrogfactory.h \
           decoder_optimfrog.h \
           optimfrogmetadatamodel.h \
           optimfroghelper.h

SOURCES += decoderoptimfrogfactory.cpp \
           decoder_optimfrog.cpp \
           optimfrogmetadatamodel.cpp \
           optimfroghelper.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/liboptimfrog/include

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/liboptimfrog/lib -lOptimFROG
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lOptimFROG
}
