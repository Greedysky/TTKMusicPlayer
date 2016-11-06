include(../../../qmmp.pri)

TEMPLATE = subdirs
win32:{
    SUBDIRS += waveout directsound wasapi
}

SUBDIRS += null

unix{
  contains(CONFIG, ALSA_PLUGIN){
      SUBDIRS += alsa
  }
}