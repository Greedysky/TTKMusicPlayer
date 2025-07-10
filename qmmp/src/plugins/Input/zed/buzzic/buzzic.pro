include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderbuzzicfactory.h \
           decoder_buzzic.h \
           buzzichelper.h \
           buzzicmetadatamodel.h

SOURCES += decoderbuzzicfactory.cpp \
           decoder_buzzic.cpp \
           buzzichelper.cpp \
           buzzicmetadatamodel.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libttk/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lbuzzic
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lbuzzic$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lbuzzic
}
