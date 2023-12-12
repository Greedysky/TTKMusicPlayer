include($$PWD/../../Visual.pri)
include($$PWD/../../common/widget.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += normalanalyzer.h \
           visualnormalanalyzerfactory.h

SOURCES += normalanalyzer.cpp \
           visualnormalanalyzerfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
