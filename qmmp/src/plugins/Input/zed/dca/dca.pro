include($$PWD/../../Input.pri)
include($$PWD/common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderdcafactory.h \
           decoder_dca.h \
           dcahelper.h

SOURCES += decoderdcafactory.cpp \
           decoder_dca.cpp \
           dcahelper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libttk/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -ldca
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -ldca$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -ldca
}
