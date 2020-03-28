include($$PWD/../../plugins.pri)

HEADERS += monotostereoplugin.h \
           effectmonotostereofactory.h

SOURCES += monotostereoplugin.cpp \
           effectmonotostereofactory.cpp

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = monotostereo

unix {
    QMAKE_CLEAN = libmonotostereo.so
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
}
