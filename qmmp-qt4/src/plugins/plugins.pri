include(../../qmmp.pri)
unix:PLUGINS_PREFIX=../../../../lib/qmmp
win32:{
    PLUGINS_PREFIX=../../../../../bin/plugins
    EXTRA_PREFIX=../../../../../extra/gcc
}
INCLUDEPATH += ../../../qmmp
