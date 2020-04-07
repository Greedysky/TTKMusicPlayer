include($$PWD/../../qmmp.pri)

EXTRA_PREFIX = $$PWD/../../../extra/gcc

unix {
    QMAKE_LIBDIR += $$OUT_PWD/../../../../lib/$$TTKMusicPlayer
    PLUGINS_PREFIX = $$OUT_PWD/../../../../lib/$$TTKMusicPlayer/qmmp
    LIBS += -lqmmp
}

win32 {
    QMAKE_LIBDIR += $$OUT_PWD/../../../../bin/$$TTKMusicPlayer
    PLUGINS_PREFIX = $$OUT_PWD/../../../../bin/$$TTKMusicPlayer/plugins
    LIBS += -lqmmp1
}

INCLUDEPATH += $$PWD/../

CONFIG += warn_on plugin lib thread link_pkgconfig
TEMPLATE = lib
