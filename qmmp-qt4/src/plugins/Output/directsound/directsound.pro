include($$PWD/../../plugins.pri)

HEADERS += outputdirectsoundfactory.h \
           outputdirectsound.h

SOURCES += outputdirectsoundfactory.cpp \
           outputdirectsound.cpp

DESTDIR = $$PLUGINS_PREFIX/Output
TARGET = $${TARGET}

LIBS += -ldxguid -lstrmiids -ldmoguids -lmsdmo -lole32 -loleaut32 -luuid -lgdi32 -ldsound -lksuser
