include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect

HEADERS += subwooferplugin.h \
           effectsubwooferfactory.h \
           settingsdialog.h

SOURCES += subwooferplugin.cpp \
           effectsubwooferfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
