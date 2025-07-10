include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += floridautism.h \
           visualfloridautismfactory.h

SOURCES += floridautism.cpp \
           visualfloridautismfactory.cpp

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
