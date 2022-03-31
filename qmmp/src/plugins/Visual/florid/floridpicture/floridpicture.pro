include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += floridpicture.h \
           visualfloridpicturefactory.h
           
SOURCES += floridpicture.cpp \
           visualfloridpicturefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
