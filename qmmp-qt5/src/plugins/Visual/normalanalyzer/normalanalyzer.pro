include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/normalanalyzer
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libnormalanalyzer.so

include(../common/Visual.pri)

HEADERS += normalanalyzer.h \
           visualnormalanalyzerfactory.h
           
SOURCES += normalanalyzer.cpp \
           visualnormalanalyzerfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
