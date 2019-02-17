include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/flashmeter
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libflashmeter.so

include(../common/Visual.pri)

HEADERS += flashmeter.h \
           visualflashmeterfactory.h
           
SOURCES += flashmeter.cpp \
           visualflashmeterfactory.cpp

RESOURCES += img.qrc

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
