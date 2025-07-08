include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect

HEADERS += silenceplugin.h \
           effectsilencefactory.h \
           settingsdialog.h

SOURCES += silenceplugin.cpp \
           effectsilencefactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
