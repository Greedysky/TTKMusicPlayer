include($$PWD/../zed.pri)

HEADERS += decoderorganyafactory.h \
           decoder_organya.h \
           organyahelper.h
    
SOURCES += decoderorganyafactory.cpp \
           decoder_organya.cpp \
           organyahelper.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/liborganya/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/liborganya/lib -lorganya$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/liborganya/lib -lorganya
}
