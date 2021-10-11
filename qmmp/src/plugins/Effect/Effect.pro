include($$PWD/../plugins.pri)

TEMPLATE = subdirs
SUBDIRS += crossfade mono monotostereo stereo

contains(CONFIG, CRYSTALIZER_PLUGIN){
    SUBDIRS += crystalizer
}

contains(CONFIG, ECHO_PLUGIN){
    SUBDIRS += echo
}

contains(CONFIG, VOICEREMOVAL_PLUGIN){
    SUBDIRS += voiceremoval
}

contains(CONFIG, BS2B_PLUGIN){
    SUBDIRS += bs2b
}

contains(CONFIG, SOXR_PLUGIN){
    SUBDIRS += soxr
}

contains(CONFIG, SRC_PLUGIN){
    SUBDIRS += srconverter
}

unix:contains(CONFIG, LADSPA_PLUGIN){
    SUBDIRS += ladspa
}
