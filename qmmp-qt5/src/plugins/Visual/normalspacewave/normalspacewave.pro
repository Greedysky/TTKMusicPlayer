include(../../plugins.pri)
include(../common/common.pri)

TARGET = $$PLUGINS_PREFIX/Visual/normalspacewave

HEADERS += normalspacewave.h \
           visualnormalspacewavefactory.h

SOURCES += normalspacewave.cpp \
           visualnormalspacewavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libnormalspacewave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
