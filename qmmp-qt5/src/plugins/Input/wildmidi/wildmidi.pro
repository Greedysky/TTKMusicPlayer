include(../../plugins.pri)

HEADERS += decoderwildmidifactory.h \
           decoder_wildmidi.h \
           wildmidihelper.h
    
SOURCES += decoder_wildmidi.cpp \
           decoderwildmidifactory.cpp \
           wildmidihelper.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/wildmidi

INCLUDEPATH += $$EXTRA_PREFIX/libwildmidi/include

unix:{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libwildmidi.so
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libwildmidi/lib/x86_64-linux-gnu -lWildMidi$$STATIC_LIBRARY_SUFFIX
}

win32:{
    LIBS += -L$$EXTRA_PREFIX/libwildmidi/lib -lWildMidi.dll
}
