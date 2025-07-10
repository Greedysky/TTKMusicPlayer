include($$PWD/../../Visual.pri)
include($$PWD/../../common/widget.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += normalline.h \
           visualnormallinefactory.h

SOURCES += normalline.cpp \
           visualnormallinefactory.cpp

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
