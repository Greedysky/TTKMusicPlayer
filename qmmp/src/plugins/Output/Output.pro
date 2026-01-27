TEMPLATE = subdirs
SUBDIRS += null

win32{
    SUBDIRS += waveout directsound wasapi
}

unix{
    SUBDIRS += pulseaudio portaudio
}

unix:!mac{
    SUBDIRS += alsa oss oss4

    PIPEWIRE_PKG = $$system(ls /usr/lib/x86_64-linux-gnu/pkgconfig | grep libpipewire)
    !isEmpty(PIPEWIRE_PKG){
        SUBDIRS += pipewire
    }
}

equals(QT_MAJOR_VERSION, 6){
    greaterThan(QT_MINOR_VERSION, 1){
        SUBDIRS += qtmultimedia
    }
}else{
    SUBDIRS += qtmultimedia
}
