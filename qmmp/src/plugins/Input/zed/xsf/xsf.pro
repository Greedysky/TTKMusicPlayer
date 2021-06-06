include($$PWD/../zed.pri)

HEADERS += decoderxsffactory.h \
           decoder_xsf.h \
           xsfhelper.h \
           xsfreader.h
    
SOURCES += decoderxsffactory.cpp \
           decoder_xsf.cpp \
           xsfhelper.cpp \
           xsfreader.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libxsf/include \
               $$EXTRA_PREFIX/libxsf/include/libxsf/mgba \

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libxsf/lib -lxsf$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libxsf/lib -lxsf
}
