include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderxsffactory.h \
           decoder_xsf.h \
           xsfhelper.h

SOURCES += decoderxsffactory.cpp \
           decoder_xsf.cpp \
           xsfhelper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libttk/include \
               $$EXTRA_PREFIX/libttk/include/libxsf/mgba \

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lxsf$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lxsf
}
