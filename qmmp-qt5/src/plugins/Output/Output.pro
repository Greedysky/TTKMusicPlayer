include(../../../qmmp.pri)

TEMPLATE = subdirs
SUBDIRS += null

win32:{
    SUBDIRS += waveout directsound wasapi
}

unix{
  contains(CONFIG, ALSA_PLUGIN){
      SUBDIRS += alsa
  }
}
