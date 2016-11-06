include(../../../qmmp.pri)

TEMPLATE = subdirs
unix:android{
    contains(CONFIG, QTMULTIMEDIA_PLUGIN){
     SUBDIRS += qtmultimedia
    }
}else{
    win32:{
        SUBDIRS += waveout directsound wasapi
    }

    SUBDIRS += null

    unix{
      contains(CONFIG, ALSA_PLUGIN){
          SUBDIRS += alsa
      }
    }
}
