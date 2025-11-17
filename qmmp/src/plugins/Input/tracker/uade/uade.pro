include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderuadefactory.h \
           decoder_uade.h \
           uadehelper.h \
           uademetadatamodel.h

SOURCES += decoderuadefactory.cpp \
           decoder_uade.cpp \
           uadehelper.cpp\
           uademetadatamodel.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libttk/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -luade
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -luade$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -luade
}
