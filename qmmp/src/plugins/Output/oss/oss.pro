include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Output

HEADERS += outputossfactory.h \
           outputoss.h \
           settingsdialog.h

SOURCES += outputossfactory.cpp \
           outputoss.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so

#DEFINES += HAVE_SYS_SOUNDCARD_H
