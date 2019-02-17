include(../../plugins.pri)

HEADERS += outputwasapifactory.h \
           outputwasapi.h

SOURCES += outputwasapifactory.cpp \
           outputwasapi.cpp

HEADERS += ../../../../src/qmmp/output.h

TARGET=$$PLUGINS_PREFIX/Output/wasapi
QMAKE_CLEAN =$$PLUGINS_PREFIX/Output/libwasapi.so

LIBS += -lstrmiids -ldmoguids -lmsdmo -lole32 -loleaut32 -luuid -lgdi32 -lksuser

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
