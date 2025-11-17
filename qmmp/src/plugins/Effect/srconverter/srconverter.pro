include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect

HEADERS += srconverter.h \
           effectsrconverterfactory.h \
           settingsdialog.h

SOURCES += srconverter.cpp \
           effectsrconverterfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_LIB_PATH/libsamplerate/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libsamplerate/lib -lsamplerate
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libsamplerate/lib -lsamplerate$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lsamplerate
}
