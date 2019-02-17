include(../../plugins.pri)

HEADERS += decodercuefactory.h \
           cueparser.h \
           decoder_cue.h \
           cuemetadatamodel.h
    
SOURCES += decoder_cue.cpp \
           decodercuefactory.cpp \
           cueparser.cpp \
           cuemetadatamodel.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/cue
QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libcue.so

INCLUDEPATH += $$EXTRA_PREFIX/libenca/include

unix{
   unix:android {
       TARGET = $$PLUGINS_PREFIX/../plugin_input_cue
       QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_cue.so
       target.path = $$LIB_DIR
   }else{
       target.path = $$LIB_DIR/qmmp/Input
   }
   INSTALLS += target
   LIBS += -L$$EXTRA_PREFIX/libenca/lib -lenca
}

win32{
   HEADERS += ../../../../src/qmmp/decoder.h \
              ../../../../src/qmmp/statehandler.h
}

contains(CONFIG, WITH_ENCA){
   DEFINES += WITH_ENCA
   win32:LIBS += -L$$EXTRA_PREFIX/libenca/lib -lenca
}
