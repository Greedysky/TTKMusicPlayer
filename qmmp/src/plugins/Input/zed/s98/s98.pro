include($$PWD/../zed.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decoders98factory.h \
           decoder_s98.h \
           s98helper.h
    
SOURCES += decoders98factory.cpp \
           decoder_s98.cpp \
           s98helper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libzed/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libzed/lib -ls98$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libzed/lib -ls98
}
