include($$PWD/../../plugins.pri)

HEADERS += decodergmefactory.h \
           decoder_gme.h \
           gmehelper.h
    
SOURCES += decodergmefactory.cpp \
           decoder_gme.cpp \
           gmehelper.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libgme/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libgme/lib -lgme$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libgme/lib -lgme
}
