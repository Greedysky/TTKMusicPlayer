include($$PWD/../../plugins.pri)

HEADERS += srconverter.h \
           effectsrconverterfactory.h \
           settingsdialog.h

SOURCES += srconverter.cpp \
           effectsrconverterfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libsoxr/include \
               $$EXTRA_PREFIX/libsamplerate/include

win32:{
    LIBS += -L$$EXTRA_PREFIX/libsamplerate/lib -lsamplerate
}

unix:{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libsamplerate/lib -lsamplerate$$STATIC_LIBRARY_SUFFIX
}
