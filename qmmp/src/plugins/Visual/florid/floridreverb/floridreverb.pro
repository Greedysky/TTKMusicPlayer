include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += floridreverb.h \
           visualfloridreverbfactory.h

SOURCES += floridreverb.cpp \
           visualfloridreverbfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
