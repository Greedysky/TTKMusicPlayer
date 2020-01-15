include(../../plugins.pri)

HEADERS += outputdirectsoundfactory.h \
           outputdirectsound.h

SOURCES += outputdirectsoundfactory.cpp \
           outputdirectsound.cpp

HEADERS += ../../../../src/qmmp/output.h

TARGET = $$PLUGINS_PREFIX/Output/directsound

LIBS += -ldxguid -lstrmiids -ldmoguids -lmsdmo -lole32 -loleaut32 -luuid -lgdi32 -ldsound -lksuser

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
