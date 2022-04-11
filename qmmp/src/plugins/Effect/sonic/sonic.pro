include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

HEADERS += sonicplugin.h \
           effectsonicfactory.h \
           settingsdialog.h

SOURCES += sonicplugin.cpp \
           effectsonicfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libzed/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libzed/lib -lsonic
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libzed/lib -lsonic$$STATIC_LIBRARY_SUFFIX
}
