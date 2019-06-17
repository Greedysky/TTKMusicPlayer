include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/floridreverberation
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libfloridreverberation.so

include(../common/common.pri)

HEADERS += floridreverberation.h \
           visualfloridreverberationfactory.h
           
SOURCES += floridreverberation.cpp \
           visualfloridreverberationfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
