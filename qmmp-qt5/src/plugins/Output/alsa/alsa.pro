include($$PWD/../../plugins.pri))

HEADERS += outputalsa.h \
           outputalsafactory.h

SOURCES += outputalsa.cpp \
           outputalsafactory.cpp

DESTDIR = $$PLUGINS_PREFIX/Output
TARGET = alsa

QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libalsa.so

INCLUDEPATH += $$EXTRA_PREFIX/libalsa/include

LIBS += -L$$EXTRA_PREFIX/libalsa/lib -lasound
