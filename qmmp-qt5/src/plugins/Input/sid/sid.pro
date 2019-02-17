include(../../plugins.pri)

HEADERS += decodersidfactory.h \
           decoder_sid.h \
           sidhelper.h
    
SOURCES += decoder_sid.cpp \
           decodersidfactory.cpp \
           sidhelper.cpp

TARGET = $$PLUGINS_PREFIX/Input/sid
QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libsid.so

INCLUDEPATH += $$EXTRA_PREFIX/libsidplayfp/include

unix {
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libsidplayfp/lib -lsidplayfp
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libsidplayfp/lib -lsidplayfp
}
