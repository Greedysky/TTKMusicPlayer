include($$PWD/additional.pri)

TEMPLATE = subdirs
SUBDIRS += ym alac vtx dca dumb v2m psf sc68

contains(CONFIG, ADPLUG_PLUGIN){
    SUBDIRS += adplug
}

contains(CONFIG, OPTIMFROG_PLUGIN){
    SUBDIRS += optimfrog
}

contains(CONFIG, SPEEX_PLUGIN){
    SUBDIRS += speex
}
