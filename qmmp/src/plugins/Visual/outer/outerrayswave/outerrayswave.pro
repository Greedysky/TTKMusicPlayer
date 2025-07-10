include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += outerrayswave.h \
           visualouterrayswavefactory.h

SOURCES += outerrayswave.cpp \
           visualouterrayswavefactory.cpp

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
