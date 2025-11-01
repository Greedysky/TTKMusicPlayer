include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderfc14factory.h \
           decoder_fc14.h \
           fc14helper.h

SOURCES += decoderfc14factory.cpp \
           decoder_fc14.cpp \
           fc14helper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libttk/include

win32{
    DEFINES += LIBFC_VERSION1
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lfc14
}

unix:!mac{
    DEFINES += LIBFC_VERSION1
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lfc14$$STATIC_LIBRARY_SUFFIX
}

mac{
    # There's no version header file provided, so we can only detect the ABI.
    FC_VERSION = $$system(grep -r "fc14dec_reinit" /opt/local/include/fc14audiodecoder.h)
    isEmpty(FC_VERSION){
        DEFINES += LIBFC_VERSION1
    }else{
        DEFINES += LIBFC_VERSION2
    }

    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lfc14audiodecoder
}
