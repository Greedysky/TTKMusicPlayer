include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += flowvumeter.h \
           visualflowvumeterfactory.h

SOURCES += flowvumeter.cpp \
           visualflowvumeterfactory.cpp

RESOURCES += flowvumeter.qrc

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
