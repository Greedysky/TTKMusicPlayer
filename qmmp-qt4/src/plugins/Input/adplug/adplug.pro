include(../../plugins.pri)

HEADERS += decoderadplugfactory.h \
           magic.h \
           decoder_adplug.h \
           adplugwrap.h \
           adplugmetadatamodel.h

SOURCES += decoderadplugfactory.cpp \
           magic.cpp \
           decoder_adplug.cpp \
           adplugwrap.cpp \
           adplugmetadatamodel.cpp

TARGET = $$PLUGINS_PREFIX/Input/adplug

INCLUDEPATH += $$EXTRA_PREFIX/libadplug/include \
               $$EXTRA_PREFIX/libadplug/include/libbinio


unix:{
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_adplug
        QMAKE_CLEAN = $$PLUGINS_PREFIX/../libplugin_input_adplug.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libadplug.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libadplug/lib -ladplug$$STATIC_LIBRARY_SUFFIX -lbinio$$STATIC_LIBRARY_SUFFIX
}

win32:{
    LIBS += -L$$EXTRA_PREFIX/libadplug/lib -ladplug -lbinio
}
