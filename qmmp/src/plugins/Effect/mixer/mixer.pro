include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect

HEADERS += mixerplugin.h \
           effectmixerfactory.h \
           settingsdialog.h

SOURCES += mixerplugin.cpp \
           effectmixerfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
