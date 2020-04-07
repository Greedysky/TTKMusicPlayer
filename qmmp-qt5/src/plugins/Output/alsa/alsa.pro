include($$PWD/../../plugins.pri))

HEADERS += outputalsa.h \
           outputalsafactory.h

SOURCES += outputalsa.cpp \
           outputalsafactory.cpp

DESTDIR = $$PLUGINS_PREFIX/Output
TARGET = $${TARGET}

QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so

INCLUDEPATH += $$EXTRA_PREFIX/libalsa/include

LIBS += -L$$EXTRA_PREFIX/libalsa/lib -lasound
