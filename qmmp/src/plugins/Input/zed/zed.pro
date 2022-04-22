include($$PWD/zed.pri)

TEMPLATE = subdirs     

contains(CONFIG, ADPLUG_PLUGIN){
    SUBDIRS += adplug
}

contains(CONFIG, ARCHIVE_PLUGIN){
    SUBDIRS += archive
}

contains(CONFIG, ASAP_PLUGIN){
    SUBDIRS += asap
}

contains(CONFIG, AYFLY_PLUGIN){
    SUBDIRS += ayfly
}

contains(CONFIG, DCA_PLUGIN){
    SUBDIRS += dca
}

contains(CONFIG, FC14_PLUGIN){
    SUBDIRS += fc14
}

contains(CONFIG, HIVELY_PLUGIN){
    SUBDIRS += hively
}

contains(CONFIG, MDX_PLUGIN){
    SUBDIRS += mdx
}

contains(CONFIG, OPTIMFROG_PLUGIN){
    SUBDIRS += optimfrog
}

contains(CONFIG, ORGANYA_PLUGIN){
    SUBDIRS += organya
}

contains(CONFIG, PSF_PLUGIN){
    SUBDIRS += psf
}

contains(CONFIG, S98_PLUGIN){
    SUBDIRS += s98
}

contains(CONFIG, SC68_PLUGIN){
    SUBDIRS += sc68
}

contains(CONFIG, SOUNDMON_PLUGIN){
    SUBDIRS += soundmon
}

contains(CONFIG, STSOUND_PLUGIN){
    SUBDIRS += stsound
}

contains(CONFIG, V2M_PLUGIN){
    SUBDIRS += v2m
}

contains(CONFIG, XSF_PLUGIN){
    SUBDIRS += xsf
}

