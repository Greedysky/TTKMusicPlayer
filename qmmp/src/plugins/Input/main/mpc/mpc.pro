include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodermpcfactory.h \
           decoder_mpc.h \
           mpcmetadatamodel.h

SOURCES += decodermpcfactory.cpp \
           decoder_mpc.cpp \
           mpcmetadatamodel.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libtaglib/include \
               $$EXTRA_LIB_PATH/libmpcdec/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libmpcdec/lib -lmpcdec \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libmpcdec/lib -lmpcdec$$STATIC_LIB_SUFFIX \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lmpcdec -ltag
}
