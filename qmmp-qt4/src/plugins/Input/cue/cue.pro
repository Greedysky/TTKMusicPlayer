include(../../plugins.pri)

HEADERS += decodercuefactory.h \
           cueparser.h \
           decoder_cue.h \
           cuemetadatamodel.h
    
SOURCES += decoder_cue.cpp \
           decodercuefactory.cpp \
           cueparser.cpp \
           cuemetadatamodel.cpp
    
win32:HEADERS += ../../../../src/qmmp/decoder.h \
                 ../../../../src/qmmp/statehandler.h
    
TARGET = $$PLUGINS_PREFIX/Input/cue
unix:QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libcue.so

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libenca/include
               
CONFIG += warn_on \
          plugin
          
TEMPLATE = lib

unix{
   isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
   target.path = $$LIB_DIR/qmmp/Input
   INSTALLS += target
   QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
   LIBS += -L$$EXTRA_PREFIX/libenca/lib -lenca -lqmmp
}

win32{
   QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
   LIBS += -lqmmp0
}

contains(CONFIG, WITH_ENCA){
   CONFIG += link_pkgconfig
   DEFINES += WITH_ENCA
   win32:LIBS += -L$$EXTRA_PREFIX/libenca/lib -lenca

}
