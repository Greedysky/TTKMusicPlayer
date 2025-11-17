include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderstsoundfactory.h \
           decoder_stsound.h \
           stsoundhelper.h

SOURCES += decoderstsoundfactory.cpp \
           decoder_stsound.cpp \
           stsoundhelper.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libttk/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lstsound
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lstsound$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lStSound
}
