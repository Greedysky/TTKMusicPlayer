include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderkssfactory.h \
           decoder_kss.h \
           ksshelper.h \
           settingsdialog.h

SOURCES += decoderkssfactory.cpp \
           decoder_kss.cpp \
           ksshelper.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_LIB_PATH/libttk/include \
               $$EXTRA_LIB_PATH/libttk/include/libkss

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lkss
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lkss$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lkss
}
