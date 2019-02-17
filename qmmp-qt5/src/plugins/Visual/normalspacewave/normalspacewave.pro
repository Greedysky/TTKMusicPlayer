include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/normalspacewave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libnormalspacewave.so

include(../common/Visual.pri)

HEADERS += normalspacewave.h \
           visualnormalspacewavefactory.h

SOURCES += normalspacewave.cpp \
           visualnormalspacewavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
