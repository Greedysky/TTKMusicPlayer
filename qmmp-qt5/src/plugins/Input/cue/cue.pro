include($$PWD/../../plugins.pri)

HEADERS += decodercuefactory.h \
           decoder_cue.h \
           cueparser.h \
           cuemetadatamodel.h
    
SOURCES += decodercuefactory.cpp \
           decoder_cue.cpp \
           cueparser.cpp \
           cuemetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = cue

INCLUDEPATH += $$EXTRA_PREFIX/libenca/include

unix{
   unix:android {
       TARGET = plugin_input_cue
       QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libplugin_input_cue.so
   }else{
       QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libcue.so
   }
   LIBS += -L$$EXTRA_PREFIX/libenca/lib -lenca$$STATIC_LIBRARY_SUFFIX
}

contains(CONFIG, WITH_ENCA){
   DEFINES += WITH_ENCA
   win32:LIBS += -L$$EXTRA_PREFIX/libenca/lib -lenca
}
