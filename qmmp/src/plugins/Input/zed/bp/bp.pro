include($$PWD/../zed.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decoderbpfactory.h \
           decoder_bp.h \
           bphelper.h
    
SOURCES += decoderbpfactory.cpp \
           decoder_bp.cpp \
           bphelper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libbp/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libbp/lib -lbp$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libbp/lib -lbp
}
