CONFIG += ordered

include(qmmp.pri)

TEMPLATE = subdirs
SUBDIRS += src/qmmp src/plugins

contains(CONFIG, BUILD_APP){
    SUBDIRS += src/buld
}
