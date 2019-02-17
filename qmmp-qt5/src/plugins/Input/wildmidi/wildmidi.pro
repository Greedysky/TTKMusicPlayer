include(../../plugins.pri)

HEADERS += decoderwildmidifactory.h \
           decoder_wildmidi.h \
           wildmidihelper.h
    
SOURCES += decoder_wildmidi.cpp \
           decoderwildmidifactory.cpp \
           wildmidihelper.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/wildmidi
QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libwildmidi.so

INCLUDEPATH += $$EXTRA_PREFIX/libwildmidi/include

unix:{
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libwildmidi/lib/x86_64-linux-gnu -lWildMidi
}

win32:{
    LIBS += -L$$EXTRA_PREFIX/libwildmidi/lib -lWildMidi.dll
}