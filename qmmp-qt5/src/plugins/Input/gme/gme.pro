include(../../plugins.pri)

HEADERS += decodergmefactory.h \
    decoder_gme.h \
    gmehelper.h \
    settingsdialog.h
SOURCES += decoder_gme.cpp \
    decodergmefactory.cpp \
    gmehelper.cpp \
    settingsdialog.cpp
TARGET = $$PLUGINS_PREFIX/Input/gme
QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libgme.so
INCLUDEPATH += ../../../
CONFIG += warn_on \
    plugin
TEMPLATE = lib



unix{
    isEmpty (LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    QMAKE_LIBDIR += ../../../../lib
    LIBS += -lqmmp \
      -L/usr/lib \
      -L/usr/local/lib \
      -I/usr/include \
      -I/usr/local/include \
      -lgme
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin

    gcc{
        INCLUDEPATH += $$EXTRA_PREFIX/libgme/include

        LIBS += -L$$EXTRA_PREFIX/libgme/lib -lgme.dll \
                -lqmmp1
    }
#    LIBS += -lqmmp0 -lgme.dll
}

FORMS += \
    settingsdialog.ui
