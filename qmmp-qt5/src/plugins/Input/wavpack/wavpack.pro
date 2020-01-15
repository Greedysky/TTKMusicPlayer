include(../../plugins.pri)

HEADERS += decoderwavpackfactory.h \
           decoder_wavpack.h \
           cueparser.h \
           wavpackmetadatamodel.h
    
SOURCES += decoder_wavpack.cpp \
           decoderwavpackfactory.cpp \
           cueparser.cpp \
           wavpackmetadatamodel.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/wavpack

INCLUDEPATH += $$EXTRA_PREFIX/libwavpack/include

unix {
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_wavpack
        QMAKE_CLEAN = $$PLUGINS_PREFIX/../libplugin_input_wavpack.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libwavpack.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libwavpack/lib -lwavpack$$STATIC_LIBRARY_SUFFIX
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libwavpack/lib -lwavpack
}
