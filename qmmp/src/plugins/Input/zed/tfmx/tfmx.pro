include($$PWD/../zed.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decodertfmxfactory.h \
           decoder_tfmx.h \
           tfmxhelper.h
    
SOURCES += decodertfmxfactory.cpp \
           decoder_tfmx.cpp \
           tfmxhelper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libzed/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libzed/lib -ltfmx$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libzed/lib -ltfmx -lws2_32
}
