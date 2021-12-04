include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decodersidfactory.h \
           decoder_sid.h \
           sidhelper.h \
           settingsdialog.h
    
SOURCES += decodersidfactory.cpp \
           decoder_sid.cpp \
           sidhelper.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libsidplayfp/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libsidplayfp/lib -lsidplayfp$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libsidplayfp/lib -lsidplayfp
}
