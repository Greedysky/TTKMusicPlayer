include($$PWD/../../plugins.pri)

HEADERS += outputnullfactory.h \
           outputnull.h

SOURCES += outputnullfactory.cpp \
           outputnull.cpp

DESTDIR = $$PLUGINS_PREFIX/Output
TARGET = null

unix {
    QMAKE_CLEAN = libnull.so
    target.path = $$LIB_DIR/qmmp/Output
    INSTALLS += target
}
