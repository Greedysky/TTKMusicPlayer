include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect

HEADERS += bitcrusherplugin.h \
           effectbitcrusherfactory.h \
           settingsdialog.h

SOURCES += bitcrusherplugin.cpp \
           effectbitcrusherfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
