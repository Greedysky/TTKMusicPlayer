include(../../plugins.pri)

HEADERS += monoplugin.h \
           effectmonofactory.h

SOURCES += monoplugin.cpp \
           effectmonofactory.cpp

TARGET = $$PLUGINS_PREFIX/Effect/mono

unix {
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Effect/libmono.so
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
}
