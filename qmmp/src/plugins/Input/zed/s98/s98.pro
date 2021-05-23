include($$PWD/../zed.pri)

HEADERS += decoders98factory.h \
           decoder_s98.h \
           s98helper.h
    
SOURCES += decoders98factory.cpp \
           decoder_s98.cpp \
           s98helper.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libs98/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libs98/lib -ls98$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libs98/lib -ls98
}
