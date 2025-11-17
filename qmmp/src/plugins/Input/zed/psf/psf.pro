include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderpsffactory.h \
           decoder_psf.h \
           psfhelper.h \
           psfmetadatamodel.h

SOURCES += decoderpsffactory.cpp \
           decoder_psf.cpp \
           psfhelper.cpp \
           psfmetadatamodel.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libttk/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lpsf
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lpsf$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lpsf
}
