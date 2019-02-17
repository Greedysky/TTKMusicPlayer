include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/normalline
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libnormalline.so

include(../common/Visual.pri)

HEADERS += normalline.h \
           visualnormallinefactory.h
           
SOURCES += normalline.cpp \
           visualnormallinefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
