include(../../plugins.pri)

HEADERS += monotostereoplugin.h \
           effectmonotostereofactory.h

SOURCES += monotostereoplugin.cpp \
           effectmonotostereofactory.cpp

TARGET = $$PLUGINS_PREFIX/Effect/monotostereo

unix {
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Effect/libmonotostereo.so
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
}
