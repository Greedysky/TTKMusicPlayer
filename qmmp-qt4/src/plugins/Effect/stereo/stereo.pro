include($$PWD/../../plugins.pri)

HEADERS += stereoplugin.h \
           effectstereofactory.h \
           settingsdialog.h

SOURCES += stereoplugin.cpp \
           effectstereofactory.cpp \
           settingsdialog.cpp

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = stereo

FORMS += settingsdialog.ui

unix {
    QMAKE_CLEAN = libstereo.so
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
