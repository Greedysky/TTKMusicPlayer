include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodercdaudiofactory.h \
           decoder_cdaudio.h \
           settingsdialog.h

SOURCES += decodercdaudiofactory.cpp \
           decoder_cdaudio.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_LIB_PATH/libcddb/include \
               $$EXTRA_LIB_PATH/libcdio/include \
               $$EXTRA_LIB_PATH/libcdio_paranoia/include

contains(CONFIG, WITH_LIBCDDB): DEFINES += WITH_LIBCDDB

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libcdio/lib -lcdio -lcdio_paranoia -lcdio_cdda
    contains(CONFIG, WITH_LIBCDDB): LIBS += -L$$EXTRA_LIB_PATH/libcddb/lib -lcddb -lm -lwinmm -mwindows -liconv -lws2_32 -lregex
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libcdio/lib -lcdio$$STATIC_LIB_SUFFIX \
            -L$$EXTRA_LIB_PATH/libcdio/lib -lcdio_paranoia$$STATIC_LIB_SUFFIX -lcdio_cdda$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lcdio -lcdio_paranoia -lcdio_cdda -lcddb
}
