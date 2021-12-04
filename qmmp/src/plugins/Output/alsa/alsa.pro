include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Output
TARGET = $${TARGET}

HEADERS += outputalsa.h \
           outputalsafactory.h \
           settingsdialog.h

SOURCES += outputalsa.cpp \
           outputalsafactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so

LIBS += -lasound
