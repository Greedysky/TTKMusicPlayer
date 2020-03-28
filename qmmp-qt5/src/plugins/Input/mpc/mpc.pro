include($$PWD/../../plugins.pri)

HEADERS += decodermpcfactory.h \
           decoder_mpc.h \
           mpcmetadatamodel.h
    
SOURCES += decoder_mpc.cpp \
           decodermpcfactory.cpp \
           mpcmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = mpc

INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libmpcdec/include

unix {
    unix:android {
        TARGET = plugin_input_mpc
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libplugin_input_mpc.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libmpc.so
    }
    LIBS += -L$$EXTRA_PREFIX/libmpcdec/lib -lmpcdec$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libmpcdec/lib -lmpcdec \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll
}
