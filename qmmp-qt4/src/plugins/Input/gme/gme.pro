include($$PWD/../../plugins.pri)

HEADERS += decodergmefactory.h \
           decoder_gme.h \
           gmehelper.h
    
SOURCES += decoder_gme.cpp \
           decodergmefactory.cpp \
           gmehelper.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = gme

INCLUDEPATH += $$EXTRA_PREFIX/libgme/include

unix{
    unix:android {
        TARGET = plugin_input_gme
        QMAKE_CLEAN = libplugin_input_gme.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = libgme.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libgme/lib -lgme$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libgme/lib -lgme.dll
}
