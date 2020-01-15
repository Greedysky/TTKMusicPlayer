include(../../plugins.pri)

HEADERS += srconverter.h \
           effectsrconverterfactory.h \
           settingsdialog.h

SOURCES += srconverter.cpp \
           effectsrconverterfactory.cpp \
           settingsdialog.cpp

TARGET = $$PLUGINS_PREFIX/Effect/srconverter

INCLUDEPATH += $$EXTRA_PREFIX/libsoxr/include \
               $$EXTRA_PREFIX/libsamplerate/include

FORMS += settingsdialog.ui

win32:{
    LIBS += -L$$EXTRA_PREFIX/libsamplerate/lib -lsamplerate
}

unix:{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Effect/libsrconverter.so
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libsamplerate/lib -lsamplerate$$STATIC_LIBRARY_SUFFIX
}
