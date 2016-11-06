include(../../plugins.pri)

HEADERS += decoderwildmidifactory.h \
           decoder_wildmidi.h \
           wildmidihelper.h
    
SOURCES += decoder_wildmidi.cpp \
           decoderwildmidifactory.cpp \
           wildmidihelper.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/wildmidi
QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libwildmidi.so

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libwildmidi/include

CONFIG += warn_on \
          plugin
    
TEMPLATE = lib

unix:{
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libwildmidi/lib/x86_64-linux-gnu -lWildMidi -lqmmp
}

win32:{
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libwildmidi/lib -lWildMidi.dll \
            -lqmmp0
}
