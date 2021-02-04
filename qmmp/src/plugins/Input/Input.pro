include($$PWD/../plugins.pri)

TEMPLATE = subdirs
SUBDIRS += additional mpeg vorbis cue sndfile wavpack

contains(CONFIG, FLAC_PLUGIN){
    SUBDIRS += flac
}

contains(CONFIG, MODPLUG_PLUGIN){
    SUBDIRS += modplug
}

contains(CONFIG, MUSEPACK_PLUGIN){
    SUBDIRS += mpc
}

contains(CONFIG, FFAP_PLUGIN){
    SUBDIRS += ffap
}

contains(CONFIG, FFMPEG_PLUGIN){
    SUBDIRS += ffmpeg
}

contains(CONFIG, GME_PLUGIN){
    SUBDIRS += gme
}

contains(CONFIG, WILDMIDI_PLUGIN){
    SUBDIRS += wildmidi
}

contains(CONFIG, OPUS_PLUGIN){
    SUBDIRS += opus
}

contains(CONFIG, AAC_PLUGIN){
    SUBDIRS += aac
}

contains(CONFIG, SID_PLUGIN){
    SUBDIRS += sid
}

contains(CONFIG, CDAUDIO_PLUGIN){
    SUBDIRS += cdaudio
}
