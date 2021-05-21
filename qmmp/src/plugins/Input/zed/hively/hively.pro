include($$PWD/../zed.pri)

HEADERS += decoderhivelyfactory.h \
           decoder_hively.h \
           hivelyhelper.h
    
SOURCES += decoderhivelyfactory.cpp \
           decoder_hively.cpp \
           hivelyhelper.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libhively/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libhively/lib -lhively$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libhively/lib -lhively
}
