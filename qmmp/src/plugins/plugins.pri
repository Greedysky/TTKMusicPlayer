include($$PWD/../../qmmp.pri)

EXTRA_PREFIX = $$PWD/../../../extra/gcc

QMAKE_LIBDIR += $$OUT_PWD/../../../../lib/$$TTKMusicPlayer
PLUGINS_PREFIX = $$OUT_PWD/../../../../lib/$$TTKMusicPlayer/plugins

unix {
    LIBS += -lqmmp
}

win32 {
    LIBS += -lqmmp1
}

INCLUDEPATH += $$PWD/../

CONFIG += warn_on plugin lib thread link_pkgconfig
TEMPLATE = lib
