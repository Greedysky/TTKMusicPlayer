include($$PWD/../zed.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decoderayflyfactory.h \
           decoder_ayfly.h \
           ayflyhelper.h
    
SOURCES += decoderayflyfactory.cpp \
           decoder_ayfly.cpp \
           ayflyhelper.cpp

DEFINES += DISABLE_AUDIO

INCLUDEPATH += $$EXTRA_PREFIX/libayfly/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libayfly/lib -layfly$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libayfly/lib -layfly
}
