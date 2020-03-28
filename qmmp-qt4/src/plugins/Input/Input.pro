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

!android{
    contains(CONFIG, FFAP_PLUGIN){
        SUBDIRS += ffap
    }

    contains(CONFIG, XMP_PLUGIN){
        SUBDIRS += xmp
    }
}

contains(CONFIG, FFMPEG_PLUGIN){
    SUBDIRS += ffmpeg
}

contains(CONFIG, GME_PLUGIN){
    SUBDIRS += gme
}

!android{
    contains(CONFIG, WILDMIDI_PLUGIN){
        SUBDIRS += wildmidi
    }
}

contains(CONFIG, OPUS_PLUGIN){
    SUBDIRS += opus
}

contains(CONFIG, AAC_PLUGIN){
    SUBDIRS += aac
}

unix:!android{
    contains(CONFIG, CDAUDIO_PLUGIN){
        SUBDIRS += cdaudio
    }

    contains(CONFIG, SID_PLUGIN){
        SUBDIRS += sid
    }
}
