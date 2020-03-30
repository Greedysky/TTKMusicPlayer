include($$PWD/additional.pri)

TEMPLATE = subdirs
SUBDIRS += ym alac tta shn vtx dca dumb

contains(CONFIG, ADPLUG_PLUGIN){
    SUBDIRS += adplug
}

contains(CONFIG, OPTIMFROG_PLUGIN){
    SUBDIRS += optimfrog
}

contains(CONFIG, SPEEX_PLUGIN){
    SUBDIRS += speex
}
