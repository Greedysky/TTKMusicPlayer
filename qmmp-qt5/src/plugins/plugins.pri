include(../../qmmp.pri)
unix:android {
    EXTRA_PREFIX=../../../../../extra/android
}else{
    EXTRA_PREFIX=../../../../../extra/gcc
}
unix:PLUGINS_PREFIX=../../../../lib/$$TTKMusicPlayer/qmmp
win32:PLUGINS_PREFIX=../../../../../bin/$$TTKMusicPlayer/plugins
INCLUDEPATH += ../../../qmmp
CONFIG += hide_symbols
