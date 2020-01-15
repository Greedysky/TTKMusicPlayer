include(../../plugins.pri)

HEADERS += outputnullfactory.h \
           outputnull.h

SOURCES += outputnullfactory.cpp \
           outputnull.cpp

TARGET = $$PLUGINS_PREFIX/Output/null

unix {
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Output/libnull.so
    target.path = $$LIB_DIR/qmmp/Output
    INSTALLS += target
}
