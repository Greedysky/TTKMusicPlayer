include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect

HEADERS += mufflerplugin.h \
           effectmufflerfactory.h \
           settingsdialog.h

SOURCES += mufflerplugin.cpp \
           effectmufflerfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
