include($$PWD/../plugins.pri)

TEMPLATE = subdirs
SUBDIRS += null qtmultimedia

win32{
    SUBDIRS += waveout directsound wasapi
}

unix{
    SUBDIRS += alsa pulseaudio oss oss4
}
