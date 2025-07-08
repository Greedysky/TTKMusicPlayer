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

INCLUDEPATH += $$EXTRA_PREFIX/libttk/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lhively
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libttk/lib -lhively$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lhively
}
