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
}

equals(QT_MAJOR_VERSION, 6){
    greaterThan(QT_MINOR_VERSION, 1){
        QT += qtmultimedia
        SUBDIRS += qtmultimedia
    }
}else{
    QT += qtmultimedia
    SUBDIRS += qtmultimedia
}
