include($$PWD/zed.pri)

TEMPLATE = subdirs
SUBDIRS += ym ayfly dca v2m psf xsf sc68 s98 asap fc14 hively mdx

contains(CONFIG, ADPLUG_PLUGIN){
    SUBDIRS += adplug
}

contains(CONFIG, OPTIMFROG_PLUGIN){
    SUBDIRS += optimfrog
}
