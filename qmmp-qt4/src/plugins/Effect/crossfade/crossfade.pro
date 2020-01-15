include(../../plugins.pri)

HEADERS += crossfadeplugin.h \
           effectcrossfadefactory.h \
           settingsdialog.h

SOURCES += crossfadeplugin.cpp \
           effectcrossfadefactory.cpp \
           settingsdialog.cpp

TARGET = $$PLUGINS_PREFIX/Effect/crossfade

FORMS += settingsdialog.ui

unix {
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Effect/libcrossfade.so
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
