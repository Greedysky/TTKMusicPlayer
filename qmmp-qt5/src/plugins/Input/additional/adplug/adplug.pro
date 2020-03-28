include($$PWD/../additional.pri)

HEADERS += decoderadplugfactory.h \
           magic.h \
           decoder_adplug.h \
           adplughelper.h \
           adplugmetadatamodel.h

SOURCES += decoderadplugfactory.cpp \
           magic.cpp \
           decoder_adplug.cpp \
           adplughelper.cpp \
           adplugmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = adplug

INCLUDEPATH += $$EXTRA_PREFIX/libadplug/include \
               $$EXTRA_PREFIX/libadplug/include/libbinio


unix:{
    unix:android {
        TARGET = plugin_input_adplug
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libplugin_input_adplug.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libadplug.so
    }
    LIBS += -L$$EXTRA_PREFIX/libadplug/lib -ladplug$$STATIC_LIBRARY_SUFFIX -lbinio$$STATIC_LIBRARY_SUFFIX
}

win32:{
    LIBS += -L$$EXTRA_PREFIX/libadplug/lib -ladplug -lbinio
}
