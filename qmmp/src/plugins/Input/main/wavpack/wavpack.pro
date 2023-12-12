include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderwavpackfactory.h \
           decoder_wavpack.h \
           wavpackmetadatamodel.h

SOURCES += decoderwavpackfactory.cpp \
           decoder_wavpack.cpp \
           wavpackmetadatamodel.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libwavpack/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libwavpack/lib -lwavpack$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libwavpack/lib -lwavpack
}
