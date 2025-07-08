include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderarchivefactory.h \
           decoder_archive.h \
           archivereader.h \
           settingsdialog.h

SOURCES += decoderarchivefactory.cpp \
           decoder_archive.cpp \
           archivereader.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
