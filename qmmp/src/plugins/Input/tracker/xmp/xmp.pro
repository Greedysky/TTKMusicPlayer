include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderxmpfactory.h \
           decoder_xmp.h \
           xmpmetadatamodel.h \
           settingsdialog.h

SOURCES += decoderxmpfactory.cpp \
           decoder_xmp.cpp \
           xmpmetadatamodel.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_LIB_PATH/libxmp/include

win32{
    contains(CONFIG, USE_STATIC_LIBRARY){
        DEFINES += LIBXMP_STATIC
    }
    LIBS += -L$$EXTRA_LIB_PATH/libxmp/lib -lxmp
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libxmp/lib -lxmp$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lxmp
}
