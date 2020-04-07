include($$PWD/../../plugins.pri)

HEADERS += decodermodplugfactory.h \
           decoder_modplug.h \
           archivereader.h \
           modplugmetadatamodel.h
    
SOURCES += decodermodplugfactory.cpp \
           decoder_modplug.cpp \
           archivereader.cpp \
           modplugmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

DEFINES += HAVE_STDINT_H \
           HAVE_INTTYPES_H
           
# Disbale gcc warnings
QMAKE_CFLAGS += -w
QMAKE_CXXFLAGS += -w

INCLUDEPATH += $$EXTRA_PREFIX/libmodplug/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libmodplug/lib -lmodplug$$STATIC_LIBRARY_SUFFIX
}

win32 {
    contains(CONFIG, USE_STATIC_LIBRARY){
        DEFINES += MODPLUG_STATIC
    }
    LIBS += -L$$EXTRA_PREFIX/libmodplug/lib -lmodplug
    DEFINES -= UNICODE
}
