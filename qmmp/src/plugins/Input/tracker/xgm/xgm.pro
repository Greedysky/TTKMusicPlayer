include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderxgmfactory.h \
           decoder_xgm.h \
           xgmhelper.h \
           settingsdialog.h

SOURCES += decoderxgmfactory.cpp \
           decoder_xgm.cpp \
           xgmhelper.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_LIB_PATH/libttk/include \
               $$EXTRA_LIB_PATH/libttk/include/libxgm \
               $$EXTRA_LIB_PATH/libttk/include/libxgm/kss

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lxgm
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lxgm$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lxgm
}
