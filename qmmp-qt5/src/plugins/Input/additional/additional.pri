include($$PWD/../../plugins.pri)

unix {
    QMAKE_LIBDIR += $$OUT_PWD/../../../../../lib/$$TTKMusicPlayer
    PLUGINS_PREFIX= $$OUT_PWD/../../../../../lib/$$TTKMusicPlayer/qmmp
}

win32 {
    QMAKE_LIBDIR += $$OUT_PWD/../../../../../bin/$$TTKMusicPlayer
    PLUGINS_PREFIX= $$OUT_PWD/../../../../../bin/$$TTKMusicPlayer/plugins
}
