include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Transports

HEADERS += mmsinputfactory.h \
           mmsinputsource.h \
           mmsstreamreader.h \
           settingsdialog.h

SOURCES += mmsinputfactory.cpp \
           mmsinputsource.cpp \
           mmsstreamreader.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
