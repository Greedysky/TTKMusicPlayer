include($$PWD/../../plugins.pri)

TEMPLATE = subdirs

contains(CONFIG, ASAP_PLUGIN){
    SUBDIRS += asap
}

contains(CONFIG, AYFLY_PLUGIN){
    SUBDIRS += ayfly
}

contains(CONFIG, FC14_PLUGIN){
    SUBDIRS += fc14
}

contains(CONFIG, HIVELY_PLUGIN){
    SUBDIRS += hively
}

contains(CONFIG, OPENMPT_PLUGIN){
    SUBDIRS += openmpt
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

contains(CONFIG, TFMX_PLUGIN){
    SUBDIRS += tfmx
}

contains(CONFIG, XMP_PLUGIN){
    SUBDIRS += xmp
}

unix:!mac{
    contains(CONFIG, UADE_PLUGIN){
        SUBDIRS += uade
    }
}

contains(CONFIG, VGMSTREAM_PLUGIN){
    SUBDIRS += vgmstream
}
