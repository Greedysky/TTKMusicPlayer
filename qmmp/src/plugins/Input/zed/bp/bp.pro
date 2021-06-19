include($$PWD/../zed.pri)

HEADERS += decoderbpfactory.h \
           decoder_bp.h \
           bphelper.h
    
SOURCES += decoderbpfactory.cpp \
           decoder_bp.cpp \
           bphelper.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libbp/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libbp/lib -lbp$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libbp/lib -lbp
}
