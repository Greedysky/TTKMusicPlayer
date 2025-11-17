include($$PWD/../../Input.pri)
include($$PWD/common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodervgmstreamfactory.h \
           decoder_vgmstream.h \
           vgmstreamhelper.h \
           settingsdialog.h

SOURCES += decodervgmstreamfactory.cpp \
           decoder_vgmstream.cpp \
           vgmstreamhelper.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_LIB_PATH/libvgmstream/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libvgmstream/lib -lvgmstream
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libvgmstream/lib -lvgmstream$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lvgmstream
}
