include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderadlmidifactory.h \
           decoder_adlmidi.h \
           adlmidihelper.h \
           adlmidimetadatamodel.h \
           settingsdialog.h

SOURCES += decoderadlmidifactory.cpp \
           decoder_adlmidi.cpp \
           adlmidihelper.cpp \
           adlmidimetadatamodel.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libadlmidi/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libadlmidi/lib -lADLMIDI
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libadlmidi/lib -lADLMIDI$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lADLMIDI
}
