include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decoderadlmidifactory.h \
           decoder_adlmidi.h \
           adlmidihelper.h \
           settingsdialog.h

SOURCES += decoderadlmidifactory.cpp \
           decoder_adlmidi.cpp \
           adlmidihelper.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libadlmidi/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libadlmidi/lib -lADLMIDI$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libadlmidi/lib -lADLMIDI
}
