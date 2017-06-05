include(../../../qmmp.pri)
TEMPLATE = subdirs

SUBDIRS += analyzer ripples
contains(CONFIG, PROJECTM_PLUGIN){
   SUBDIRS += projectm
}
