include($$PWD/../../plugins.pri)

TEMPLATE = subdirs

SUBDIRS += asap
SUBDIRS += ayfly
SUBDIRS += fc14
SUBDIRS += hively
SUBDIRS += openmpt
SUBDIRS += sc68
SUBDIRS += soundmon
SUBDIRS += stsound
SUBDIRS += tfmx
SUBDIRS += xmp

unix:!mac:SUBDIRS += uade

contains(CONFIG, VGMSTREAM_PLUGIN){
    SUBDIRS += vgmstream
}
