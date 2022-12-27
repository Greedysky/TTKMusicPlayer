include($$PWD/../plugins.pri)

TEMPLATE = subdirs
SUBDIRS += null

win32{
    SUBDIRS += waveout directsound wasapi
}

unix{
    SUBDIRS += alsa pulseaudio oss oss4
}
