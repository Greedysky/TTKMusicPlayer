include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodercdaudiofactory.h \
           decoder_cdaudio.h \
           settingsdialog.h

SOURCES += decodercdaudiofactory.cpp \
           decoder_cdaudio.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libcddb/include \
               $$EXTRA_PREFIX/libcdio/include \
               $$EXTRA_PREFIX/libcdio_paranoia/include

contains(CONFIG, WITH_LIBCDDB): DEFINES += WITH_LIBCDDB

win32{
    LIBS += -L$$EXTRA_PREFIX/libcdio/lib -lcdio -lcdio_paranoia -lcdio_cdda
    contains(CONFIG, WITH_LIBCDDB): LIBS += -L$$EXTRA_PREFIX/libcddb/lib -lcddb -lm -lwinmm -mwindows -liconv -lws2_32 -lregex
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libcdio/lib -lcdio$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libcdio/lib -lcdio_paranoia$$STATIC_LIBRARY_SUFFIX -lcdio_cdda$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lcdio -lcdio_paranoia -lcdio_cdda
}
