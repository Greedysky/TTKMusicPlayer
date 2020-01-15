include(../../plugins.pri)

HEADERS += stereoplugin.h \
           effectstereofactory.h \
           settingsdialog.h

SOURCES += stereoplugin.cpp \
           effectstereofactory.cpp \
           settingsdialog.cpp

TARGET = $$PLUGINS_PREFIX/Effect/stereo

FORMS += settingsdialog.ui

unix {
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Effect/libstereo.so
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
