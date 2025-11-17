include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderhivelyfactory.h \
           decoder_hively.h \
           hivelyhelper.h \
           hivelymetadatamodel.h

SOURCES += decoderhivelyfactory.cpp \
           decoder_hively.cpp \
           hivelyhelper.cpp \
           hivelymetadatamodel.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libttk/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lhively
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lhively$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lhively
}
