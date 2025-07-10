include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoders98factory.h \
           decoder_s98.h \
           s98helper.h \
           s98metadatamodel.h

SOURCES += decoders98factory.cpp \
           decoder_s98.cpp \
           s98helper.cpp \
           s98metadatamodel.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libttk/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -ls98
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -ls98$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -ls98
}
