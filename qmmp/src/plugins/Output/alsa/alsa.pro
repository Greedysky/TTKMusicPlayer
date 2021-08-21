include($$PWD/../../plugins.pri)

HEADERS += outputalsa.h \
           outputalsafactory.h \
           settingsdialog.h

SOURCES += outputalsa.cpp \
           outputalsafactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

DESTDIR = $$PLUGINS_PREFIX/Output
TARGET = $${TARGET}

QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so

LIBS += -lasound
