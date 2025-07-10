include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodersoundmonfactory.h \
           decoder_soundmon.h \
           soundmonhelper.h

SOURCES += decodersoundmonfactory.cpp \
           decoder_soundmon.cpp \
           soundmonhelper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libttk/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lsoundmon
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lsoundmon$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lsoundmon
}
