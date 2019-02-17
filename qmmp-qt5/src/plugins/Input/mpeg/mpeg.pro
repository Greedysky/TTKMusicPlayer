include(../../plugins.pri)

HEADERS += decodermpegfactory.h \
	   tagextractor.h \
	   mpegmetadatamodel.h

SOURCES += decodermpegfactory.cpp \
	   tagextractor.cpp \
	   mpegmetadatamodel.cpp

TARGET = $$PLUGINS_PREFIX/Input/mpeg
QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libmpeg.so

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
    target.path = $$PLUGIN_DIR/Input
    INSTALLS += target
    contains(CONFIG, WITH_MAD):{
        LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
                -L$$EXTRA_PREFIX/libmad/lib -lmad
    }
    contains(CONFIG, WITH_MPG123):{
        LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
                -L$$EXTRA_PREFIX/libmad/lib -lmpg123
    }
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll \
            -L$$EXTRA_PREFIX/libmad/lib -lmad
}
