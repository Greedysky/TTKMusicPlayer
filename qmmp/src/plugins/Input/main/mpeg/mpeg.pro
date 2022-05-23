include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodermpegfactory.h \
	   tagextractor.h \
	   mpegmetadatamodel.h \
	   settingsdialog.h

SOURCES += decodermpegfactory.cpp \
	   tagextractor.cpp \
	   mpegmetadatamodel.cpp \
	   settingsdialog.cpp

FORMS += settingsdialog.ui

contains(CONFIG, WITH_MAD){
    INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
                   $$EXTRA_PREFIX/libmad/include
    HEADERS += decoder_mad.h
    SOURCES += decoder_mad.cpp
    DEFINES += WITH_MAD
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    contains(CONFIG, WITH_MAD):{
        LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX \
                -L$$EXTRA_PREFIX/libmad/lib -lmad$$STATIC_LIBRARY_SUFFIX
    }
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
            -L$$EXTRA_PREFIX/libmad/lib -lmad
}
