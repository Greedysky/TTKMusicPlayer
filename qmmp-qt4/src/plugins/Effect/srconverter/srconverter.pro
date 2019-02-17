include(../../plugins.pri)

HEADERS += srconverter.h \
           effectsrconverterfactory.h \
           settingsdialog.h

SOURCES += srconverter.cpp \
           effectsrconverterfactory.cpp \
           settingsdialog.cpp

TARGET=$$PLUGINS_PREFIX/Effect/srconverter
QMAKE_CLEAN =$$PLUGINS_PREFIX/Effect/libsrconverter.so

INCLUDEPATH += $$EXTRA_PREFIX/libsoxr/include \
               $$EXTRA_PREFIX/libsamplerate/include

FORMS += settingsdialog.ui

unix {
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
}

LIBS += -L$$EXTRA_PREFIX/libsamplerate/lib -lsamplerate
