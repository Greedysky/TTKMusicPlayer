include($$PWD/../../plugins.pri)

HEADERS += monoplugin.h \
           effectmonofactory.h

SOURCES += monoplugin.cpp \
           effectmonofactory.cpp

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = mono

unix {
    QMAKE_CLEAN = libmono.so
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
}
