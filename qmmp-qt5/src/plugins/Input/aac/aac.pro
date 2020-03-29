include($$PWD/../../plugins.pri)

HEADERS += decoderaacfactory.h \
           decoder_aac.h \
           aacfile.h
          
SOURCES += decoderaacfactory.cpp \
           decoder_aac.cpp \
           aacfile.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = aac

INCLUDEPATH += $$EXTRA_PREFIX/libfaad2/include \
               $$EXTRA_PREFIX/libtaglib/include

unix:{
    unix:android {
        TARGET = plugin_input_aac
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libplugin_input_aac.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libaac.so
    }
    LIBS += -L$$EXTRA_PREFIX/libfaad2/lib -lfaad$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}

win32:{
    LIBS += -L$$EXTRA_PREFIX/libfaad2/lib -lfaad \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll
}
