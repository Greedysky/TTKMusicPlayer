include(../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Visual/normalflowwave

include(../common/common.pri)

HEADERS += normalflowwave.h \
           visualnormalflowwavefactory.h

SOURCES += normalflowwave.cpp \
           visualnormalflowwavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libnormalflowwave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
