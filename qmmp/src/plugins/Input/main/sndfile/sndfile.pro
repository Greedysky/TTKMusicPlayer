include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodersndfilefactory.h \
           decoder_sndfile.h \
           sndfilemetadatamodel.h

SOURCES += decodersndfilefactory.cpp \
           decoder_sndfile.cpp \
           sndfilemetadatamodel.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libsndfile/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libsndfile/lib -lsndfile
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libsndfile/lib -lsndfile$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lsndfile
}
