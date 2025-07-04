include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Output

HEADERS += outputportaudiofactory.h \
           outputportaudio.h \
           settingsdialog.h

SOURCES += outputportaudiofactory.cpp \
           outputportaudio.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so

LIBS += -lportaudio
