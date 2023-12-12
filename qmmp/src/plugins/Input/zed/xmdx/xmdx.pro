include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderxmdxfactory.h \
           decoder_xmdx.h \
           xmdxhelper.h

SOURCES += decoderxmdxfactory.cpp \
           decoder_xmdx.cpp \
           xmdxhelper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libttk/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lxmdx$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lxmdx -lws2_32
}
