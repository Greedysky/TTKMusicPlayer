CONFIG += ordered

include(qmmp.pri)

TEMPLATE = subdirs
SUBDIRS += src/qmmp src/plugins

contains(CONFIG, BUILD_DEMO){
    SUBDIRS += src/demo
}
