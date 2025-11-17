include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodersndfilefactory.h \
           decoder_sndfile.h \
           sndfilemetadatamodel.h

SOURCES += decodersndfilefactory.cpp \
           decoder_sndfile.cpp \
           sndfilemetadatamodel.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libtaglib/include \
               $$EXTRA_LIB_PATH/libsndfile/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libsndfile/lib -lsndfile \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libsndfile/lib -lsndfile$$STATIC_LIB_SUFFIX \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lsndfile -ltag
}
