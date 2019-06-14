include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/normalewave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libnormalewave.so

include(../common/common.pri)

HEADERS += normalewave.h \
           visualnormalewavefactory.h
           
SOURCES += normalewave.cpp \
           visualnormalewavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
