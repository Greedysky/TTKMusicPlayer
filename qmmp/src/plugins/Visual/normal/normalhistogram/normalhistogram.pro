include($$PWD/../../Visual.pri)
include($$PWD/../../common/widget.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += normalhistogram.h \
           visualnormalhistogramfactory.h

SOURCES += normalhistogram.cpp \
           visualnormalhistogramfactory.cpp

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
