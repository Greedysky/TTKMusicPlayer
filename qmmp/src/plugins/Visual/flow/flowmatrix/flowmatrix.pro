include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += matrix.h \
           flowmatrix.h \
           visualflowmatrixfactory.h

SOURCES += matrix.cpp \
           flowmatrix.cpp \
           visualflowmatrixfactory.cpp

RESOURCES += flowmatrix.qrc

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
