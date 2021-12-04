include($$PWD/../zed.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decodersc68factory.h \
           decoder_sc68.h \
           sc68helper.h
    
SOURCES += decodersc68factory.cpp \
           decoder_sc68.cpp \
           sc68helper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libsc68/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libsc68/lib -lsc68$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libsc68/lib -lsc68
}
