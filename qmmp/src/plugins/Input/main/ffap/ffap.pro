include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderffapfactory.h \
           ffap.h \
           decoder_ffap.h \
           ffapmetadatamodel.h \
           decoder_ffapcue.h

SOURCES += decoderffapfactory.cpp \
           ffap.c \
           decoder_ffap.cpp \
           ffapmetadatamodel.cpp \
           decoder_ffapcue.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libtaglib/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag$$STATIC_LIB_SUFFIX -lgcc
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -ltag
}
