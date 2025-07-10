include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderayflyfactory.h \
           decoder_ayfly.h \
           ayflyhelper.h

SOURCES += decoderayflyfactory.cpp \
           decoder_ayfly.cpp \
           ayflyhelper.cpp

DEFINES += DISABLE_AUDIO

INCLUDEPATH += $$EXTRA_PREFIX/libttk/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -layfly
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -layfly$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -layfly
}
