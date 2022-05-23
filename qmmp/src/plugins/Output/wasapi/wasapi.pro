include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Output

HEADERS += outputwasapifactory.h \
           outputwasapi.h \
           settingsdialog.h

SOURCES += outputwasapifactory.cpp \
           outputwasapi.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

LIBS += -lstrmiids -ldmoguids -lmsdmo -lole32 -loleaut32 -luuid -lgdi32 -lksuser
