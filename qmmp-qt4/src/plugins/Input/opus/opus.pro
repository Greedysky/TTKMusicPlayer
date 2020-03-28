include($$PWD/../../plugins.pri)

HEADERS += decoderopusfactory.h \
           decoder_opus.h \
           opusmetadatamodel.h
    
SOURCES += decoder_opus.cpp \
           decoderopusfactory.cpp \
           opusmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = opus

INCLUDEPATH += $$EXTRA_PREFIX/libopusfile/include \
               $$EXTRA_PREFIX/libogg/include \
               $$EXTRA_PREFIX/libtaglib/include

unix {
    unix:android {
        TARGET = plugin_input_opus
        QMAKE_CLEAN = $$DESTDIR/libplugin_input_opus.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/libopus.so
    }
    LIBS += -L$$EXTRA_PREFIX/libopusfile/lib -lopusfile$$STATIC_LIBRARY_SUFFIX -lopus$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libogg/lib -logg$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libopusfile/lib -lopusfile -lopus \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll -lm
}
