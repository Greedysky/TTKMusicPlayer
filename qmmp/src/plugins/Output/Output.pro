TEMPLATE = subdirs
SUBDIRS += null qtmultimedia

win32{
    SUBDIRS += waveout directsound wasapi
}

unix{
    SUBDIRS += pulseaudio portaudio
}

unix:!mac{
    SUBDIRS += alsa oss oss4
}
