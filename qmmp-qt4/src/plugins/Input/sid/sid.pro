include($$PWD/../../plugins.pri)

HEADERS += decodersidfactory.h \
           decoder_sid.h \
           sidhelper.h
    
SOURCES += decoder_sid.cpp \
           decodersidfactory.cpp \
           sidhelper.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = sid

INCLUDEPATH += $$EXTRA_PREFIX/libsidplayfp/include

unix {
    QMAKE_CLEAN = libsid.so
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libsidplayfp/lib -lsidplayfp$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libsidplayfp/lib -lsidplayfp
}
