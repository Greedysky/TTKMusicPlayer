include($$PWD/../../plugins.pri)

QT += network
DESTDIR = $$PLUGINS_PREFIX/Transports

HEADERS += networkinputfactory.h \
           networkinputsource.h \
           networkstreamreader.h \
           settingsdialog.h

SOURCES += networkinputfactory.cpp \
           networkinputsource.cpp \
           networkstreamreader.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
