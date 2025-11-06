include($$PWD/../../Input.pri)
include($$PWD/common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodervgmstreamfactory.h \
           decoder_vgmstream.h \
           vgmstreamhelper.h

SOURCES += decodervgmstreamfactory.cpp \
           decoder_vgmstream.cpp \
           vgmstreamhelper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libvgmstream/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libvgmstream/lib -lvgmstream
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libvgmstream/lib -lvgmstream$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lvgmstream
}
