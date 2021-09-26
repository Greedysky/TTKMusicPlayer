include($$PWD/../plugins.pri)

TEMPLATE = subdirs

SUBDIRS += crossfade stereo mono monotostereo

contains(CONFIG, BS2B_PLUGIN):SUBDIRS += bs2b
contains(CONFIG, SOXR_PLUGIN):SUBDIRS += soxr
contains(CONFIG, SRC_PLUGIN):SUBDIRS += srconverter

unix:contains(CONFIG, LADSPA_PLUGIN):SUBDIRS += ladspa
