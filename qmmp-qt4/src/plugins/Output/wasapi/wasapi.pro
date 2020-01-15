include(../../plugins.pri)

HEADERS += outputwasapifactory.h \
           outputwasapi.h

SOURCES += outputwasapifactory.cpp \
           outputwasapi.cpp

HEADERS += ../../../../src/qmmp/output.h

TARGET = $$PLUGINS_PREFIX/Output/wasapi

LIBS += -lstrmiids -ldmoguids -lmsdmo -lole32 -loleaut32 -luuid -lgdi32 -lksuser

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
