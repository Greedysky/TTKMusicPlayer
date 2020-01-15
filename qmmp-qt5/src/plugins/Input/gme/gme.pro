include(../../plugins.pri)

HEADERS += decodergmefactory.h \
           decoder_gme.h \
           gmehelper.h
    
SOURCES += decoder_gme.cpp \
           decodergmefactory.cpp \
           gmehelper.cpp

TARGET = $$PLUGINS_PREFIX/Input/gme

INCLUDEPATH += $$EXTRA_PREFIX/libgme/include

unix{
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_gme
        QMAKE_CLEAN = $$PLUGINS_PREFIX/../libplugin_input_gme.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libgme.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libgme/lib -lgme$$STATIC_LIBRARY_SUFFIX
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libgme/lib -lgme.dll
}
