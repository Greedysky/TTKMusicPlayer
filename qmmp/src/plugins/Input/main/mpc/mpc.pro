include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodermpcfactory.h \
           decoder_mpc.h \
           mpcmetadatamodel.h

SOURCES += decodermpcfactory.cpp \
           decoder_mpc.cpp \
           mpcmetadatamodel.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libmpcdec/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libmpcdec/lib -lmpcdec \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libmpcdec/lib -lmpcdec$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lmpcdec -ltag
}
