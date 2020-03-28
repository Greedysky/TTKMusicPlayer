include($$PWD/../../plugins.pri)

HEADERS += decodermodplugfactory.h \
           decoder_modplug.h \
           archivereader.h \
           modplugmetadatamodel.h
    
SOURCES += decoder_modplug.cpp \
           decodermodplugfactory.cpp \
           archivereader.cpp \
           modplugmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = modplug

DEFINES += HAVE_STDINT_H \
           HAVE_INTTYPES_H
           
INCLUDEPATH += $$EXTRA_PREFIX/libmodplug/include

unix {
    unix:android {
        TARGET = plugin_input_modplug
        QMAKE_CLEAN = libplugin_input_modplug.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = libmodplug.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libmodplug/lib -lmodplug$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libmodplug/lib -lmodplug
    DEFINES -= UNICODE
}
