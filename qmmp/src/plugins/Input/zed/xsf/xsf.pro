include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderxsffactory.h \
           decoder_xsf.h \
           xsfhelper.h \
           xsfmetadatamodel.h

SOURCES += decoderxsffactory.cpp \
           decoder_xsf.cpp \
           xsfhelper.cpp \
           xsfmetadatamodel.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libttk/include \
               $$EXTRA_LIB_PATH/libttk/include/libxsf/mgba

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lxsf
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lxsf$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lxsf
}
