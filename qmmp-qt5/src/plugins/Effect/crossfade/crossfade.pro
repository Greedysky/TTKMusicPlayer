include(../../plugins.pri)

HEADERS += crossfadeplugin.h \
           effectcrossfadefactory.h \
           settingsdialog.h

SOURCES += crossfadeplugin.cpp \
           effectcrossfadefactory.cpp \
           settingsdialog.cpp

TARGET =$$PLUGINS_PREFIX/Effect/crossfade
QMAKE_CLEAN =$$PLUGINS_PREFIX/Effect/libcrossfade.so

FORMS += settingsdialog.ui

unix {
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
