include($$PWD/../../plugins.pri)

HEADERS += decodermpegfactory.h \
	   tagextractor.h \
	   mpegmetadatamodel.h

SOURCES += decodermpegfactory.cpp \
	   tagextractor.cpp \
	   mpegmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = mpeg

contains(CONFIG, WITH_MAD){
    INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
                   $$EXTRA_PREFIX/libmad/include
    HEADERS += decoder_mad.h
    SOURCES += decoder_mad.cpp
    DEFINES += WITH_MAD
}

unix:contains(CONFIG, WITH_MPG123){
    HEADERS += decoder_mpg123.h
    SOURCES += decoder_mpg123.cpp
    DEFINES += WITH_MPG123
}

unix {
    QMAKE_CLEAN = $$DESTDIR/libmpeg.so
    contains(CONFIG, WITH_MAD):{
        LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX \
                -L$$EXTRA_PREFIX/libmad/lib -lmad$$STATIC_LIBRARY_SUFFIX
    }
    contains(CONFIG, WITH_MPG123):{
        LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX \
                -L$$EXTRA_PREFIX/libmad/lib -lmpg123$$STATIC_LIBRARY_SUFFIX
    }
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll \
            -L$$EXTRA_PREFIX/libmad/lib -lmad
}
