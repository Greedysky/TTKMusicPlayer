include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Output

HEADERS += outputoss4factory.h \
           outputoss4.h \
           settingsdialog.h

SOURCES += outputoss4factory.cpp \
           outputoss4.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so

#DEFINES += HAVE_SYS_SOUNDCARD_H
