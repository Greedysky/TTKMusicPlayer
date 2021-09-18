include($$PWD/../../plugins.pri)

HEADERS += decodersndfilefactory.h \
           decoder_sndfile.h
           
SOURCES += decodersndfilefactory.cpp \
           decoder_sndfile.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libsndfile/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libsndfile/lib -lsndfile$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libsndfile/lib -lsndfile
}
