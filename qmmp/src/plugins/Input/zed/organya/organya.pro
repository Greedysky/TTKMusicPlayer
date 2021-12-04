include($$PWD/../zed.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decoderorganyafactory.h \
           decoder_organya.h \
           organyahelper.h
    
SOURCES += decoderorganyafactory.cpp \
           decoder_organya.cpp \
           organyahelper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/liborganya/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/liborganya/lib -lorganya$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/liborganya/lib -lorganya
}
