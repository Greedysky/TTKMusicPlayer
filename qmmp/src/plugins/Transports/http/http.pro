include($$PWD/../../plugins.pri)

QT += network
DESTDIR = $$PLUGINS_PREFIX/Transports

HEADERS += httpinputfactory.h \
           httpinputsource.h \
           httpstreamreader.h \
           settingsdialog.h

SOURCES += httpinputfactory.cpp \
           httpinputsource.cpp \
           httpstreamreader.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
