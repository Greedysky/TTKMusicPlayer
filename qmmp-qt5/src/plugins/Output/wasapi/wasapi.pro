include($$PWD/../../plugins.pri)

HEADERS += outputwasapifactory.h \
           outputwasapi.h

SOURCES += outputwasapifactory.cpp \
           outputwasapi.cpp

DESTDIR = $$PLUGINS_PREFIX/Output
TARGET = wasapi

LIBS += -lstrmiids -ldmoguids -lmsdmo -lole32 -loleaut32 -luuid -lgdi32 -lksuser

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
