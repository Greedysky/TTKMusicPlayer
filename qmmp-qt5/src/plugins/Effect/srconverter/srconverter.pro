include($$PWD/../../plugins.pri)

HEADERS += srconverter.h \
           effectsrconverterfactory.h \
           settingsdialog.h

SOURCES += srconverter.cpp \
           effectsrconverterfactory.cpp \
           settingsdialog.cpp

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = srconverter

INCLUDEPATH += $$EXTRA_PREFIX/libsoxr/include \
               $$EXTRA_PREFIX/libsamplerate/include

FORMS += settingsdialog.ui

win32:{
    LIBS += -L$$EXTRA_PREFIX/libsamplerate/lib -lsamplerate
}

unix:{
    QMAKE_CLEAN = libsrconverter.so
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libsamplerate/lib -lsamplerate$$STATIC_LIBRARY_SUFFIX
}
