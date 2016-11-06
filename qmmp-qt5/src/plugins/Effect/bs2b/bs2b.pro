include(../../plugins.pri)

HEADERS += bs2bplugin.h \
           effectbs2bfactory.h \
           settingsdialog.h

SOURCES += bs2bplugin.cpp \
           effectbs2bfactory.cpp \
           settingsdialog.cpp

TARGET =$$PLUGINS_PREFIX/Effect/bs2b

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libbs2b/include
               
CONFIG += warn_on \
          plugin \
          link_pkgconfig

TEMPLATE = lib

FORMS += settingsdialog.ui

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libbs2b/lib -lbs2b -lqmmp
}

win32 {
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libbs2b/lib -lbs2b -lqmmp1
}
