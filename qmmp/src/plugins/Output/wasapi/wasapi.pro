include($$PWD/../../plugins.pri)

HEADERS += outputwasapifactory.h \
           outputwasapi.h \
           settingsdialog.h

SOURCES += outputwasapifactory.cpp \
           outputwasapi.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

DESTDIR = $$PLUGINS_PREFIX/Output
TARGET = $${TARGET}

LIBS += -lstrmiids -ldmoguids -lmsdmo -lole32 -loleaut32 -luuid -lgdi32 -lksuser
