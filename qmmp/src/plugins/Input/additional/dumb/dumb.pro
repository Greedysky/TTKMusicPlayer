include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decoderdumbfactory.h \
           decoder_dumb.h \
           dumbhelper.h \
           dumbmetadatamodel.h

SOURCES += decoderdumbfactory.cpp \
           decoder_dumb.cpp \
           dumbhelper.cpp \
           dumbmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libdumb/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libdumb/lib -ldumb$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libdumb/lib -ldumb
}
