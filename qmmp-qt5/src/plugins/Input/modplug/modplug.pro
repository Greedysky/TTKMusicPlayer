include(../../plugins.pri)

HEADERS += decodermodplugfactory.h \
           decoder_modplug.h \
           archivereader.h \
           modplugmetadatamodel.h
    
SOURCES += decoder_modplug.cpp \
           decodermodplugfactory.cpp \
           archivereader.cpp \
           modplugmetadatamodel.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/modplug
QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libmodplug.so

DEFINES += HAVE_STDINT_H \
           HAVE_INTTYPES_H
           
INCLUDEPATH += $$EXTRA_PREFIX/libmodplug/include

unix {
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_modplug
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_modplug.so
        target.path = $$LIB_DIR
    }else{
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libmodplug/lib -lmodplug
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libmodplug/lib -lmodplug
    DEFINES -= UNICODE
}
