include($$PWD/../../plugins.pri)

HEADERS += decoderwildmidifactory.h \
           decoder_wildmidi.h \
           wildmidihelper.h \
           settingsdialog.h
    
SOURCES += decoderwildmidifactory.cpp \
           decoder_wildmidi.cpp \
           wildmidihelper.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libwildmidi/include

unix:{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libwildmidi/lib -lWildMidi$$STATIC_LIBRARY_SUFFIX
}

win32:{
    contains(CONFIG, USE_STATIC_LIBRARY){
        DEFINES += WILDMIDI_STATIC
    }
    LIBS += -L$$EXTRA_PREFIX/libwildmidi/lib -lWildMidi
}
