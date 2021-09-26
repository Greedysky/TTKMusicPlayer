include($$PWD/../../plugins.pri)

HEADERS += bs2bplugin.h \
           effectbs2bfactory.h \
           settingsdialog.h

SOURCES += bs2bplugin.cpp \
           effectbs2bfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libbs2b/include

win32:{
    LIBS += -L$$EXTRA_PREFIX/libbs2b/lib -lbs2b
}

unix:{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libbs2b/lib -lbs2b$$STATIC_LIBRARY_SUFFIX
}
