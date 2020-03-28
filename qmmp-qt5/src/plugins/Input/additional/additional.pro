include($$PWD/additional.pri)

TEMPLATE = subdirs
SUBDIRS += ym alac tta shn

contains(CONFIG, ADPLUG_PLUGIN){
    SUBDIRS += adplug
}

contains(CONFIG, OPTIMFROG_PLUGIN){
    SUBDIRS += optimfrog
}

contains(CONFIG, SPEEX_PLUGIN){
    SUBDIRS += speex
}
