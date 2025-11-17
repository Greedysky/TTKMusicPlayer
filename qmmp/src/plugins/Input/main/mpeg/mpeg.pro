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

INCLUDEPATH += $$EXTRA_LIB_PATH/libtaglib/include \
               $$EXTRA_LIB_PATH/libmpg123/include

HEADERS += decoder_mpg123.h
SOURCES += decoder_mpg123.cpp

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag \
            -L$$EXTRA_LIB_PATH/libmpg123/lib -lmpg123 -lshlwapi
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag$$STATIC_LIB_SUFFIX \
            -L$$EXTRA_LIB_PATH/libmpg123/lib -lmpg123$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -ltag -lmpg123
}
