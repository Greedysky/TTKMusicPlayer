include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/floridmeter
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libfloridmeter.so

include(../common/common.pri)

HEADERS += floridmeter.h \
           visualfloridmeterfactory.h
           
SOURCES += floridmeter.cpp \
           visualfloridmeterfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
