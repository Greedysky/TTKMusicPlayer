include($$PWD/../../plugins.pri)

HEADERS += decodercuefactory.h \
           cueparser.h \
           decoder_cue.h \
           cuemetadatamodel.h
    
SOURCES += decoder_cue.cpp \
           decodercuefactory.cpp \
           cueparser.cpp \
           cuemetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = cue

INCLUDEPATH += $$EXTRA_PREFIX/libenca/include

unix{
   unix:android {
       TARGET = plugin_input_cue
       QMAKE_CLEAN = libplugin_input_cue.so
       target.path = $$LIB_DIR
   }else{
       QMAKE_CLEAN = libcue.so
       target.path = $$LIB_DIR/qmmp/Input
   }
   INSTALLS += target
   LIBS += -L$$EXTRA_PREFIX/libenca/lib -lenca$$STATIC_LIBRARY_SUFFIX
}

contains(CONFIG, WITH_ENCA){
   DEFINES += WITH_ENCA
   win32:LIBS += -L$$EXTRA_PREFIX/libenca/lib -lenca
}
