include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodermpegfactory.h \
           tagextractor.h \
           mpegmetadatamodel.h \
           settingsdialog.h \
           checker.h

SOURCES += decodermpegfactory.cpp \
           tagextractor.cpp \
           mpegmetadatamodel.cpp \
           settingsdialog.cpp \
           checker.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libmpg123/include

HEADERS += decoder_mpg123.h
SOURCES += decoder_mpg123.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libmpg123/lib -lmpg123$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
            -L$$EXTRA_PREFIX/libmpg123/lib -lmpg123 -lshlwapi
}
