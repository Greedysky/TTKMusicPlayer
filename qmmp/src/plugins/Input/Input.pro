include($$PWD/../plugins.pri)

TEMPLATE = subdirs
SUBDIRS += main

contains(CONFIG, TRACKER_PLUGIN){
    SUBDIRS += tracker
}

contains(CONFIG, ZED_PLUGIN){
    SUBDIRS += zed
}
