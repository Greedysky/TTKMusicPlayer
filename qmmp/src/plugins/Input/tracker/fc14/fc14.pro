include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderfc14factory.h \
           decoder_fc14.h \
           fc14helper.h

SOURCES += decoderfc14factory.cpp \
           decoder_fc14.cpp \
           fc14helper.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libttk/include

win32{
    LIB_INCLUDE = $$EXTRA_LIB_PATH/libttk/include/libfc14/fc14audiodecoder.h
    LIB_VERSION = $$system(findstr /r "fc14dec_reinit" $$replace(LIB_INCLUDE, /, \\))
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lfc14
}

unix:!mac{
    LIB_VERSION = $$system(grep -r "fc14dec_reinit" $$EXTRA_LIB_PATH/libttk/include/libfc14/fc14audiodecoder.h)
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lfc14$$STATIC_LIB_SUFFIX
}

mac{
    LIB_VERSION = $$system(grep -r "fc14dec_reinit" /opt/local/include/fc14audiodecoder.h)
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lfc14audiodecoder
}

# Check version
# Note: There's no version header file provided, so we can only detect the ABI.
isEmpty(LIB_VERSION){
    DEFINES += LIBFC_VERSION_OLD
    message("Found libfc14audiodecoder version 1.x")
}else{
    DEFINES += LIBFC_VERSION_NEW
    message("Found libfc14audiodecoder version 2.x")
}
