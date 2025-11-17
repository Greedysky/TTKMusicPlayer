include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderaacfactory.h \
           decoder_aac.h \
           aacfile.h

SOURCES += decoderaacfactory.cpp \
           decoder_aac.cpp \
           aacfile.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libfaad2/include \
               $$EXTRA_LIB_PATH/libtaglib/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libfaad2/lib -lfaad \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libfaad2/lib -lfaad$$STATIC_LIB_SUFFIX \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lfaad -ltag
}
