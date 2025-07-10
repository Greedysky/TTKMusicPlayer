include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += wavevoice.h \
           visualwavevoicefactory.h

SOURCES += wavevoice.cpp \
           visualwavevoicefactory.cpp

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
