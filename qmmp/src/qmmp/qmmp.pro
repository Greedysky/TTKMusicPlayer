include($$PWD/../../qmmp.pri)

HEADERS += \
    buffer.h \
    decoder.h \
    output.h \
    outputfactory.h \
    equ/iir_cfs.h \
    equ/iir_fpu.h \
    equ/iir.h \
    decoderfactory.h \
    soundcore.h \
    visual.h \
    visualpalette.h \
    visualfactory.h \
    effect.h \
    effectfactory.h \
    statehandler.h \
    qmmp.h \
    metadatamodel.h \
    tagmodel.h \
    abstractengine.h \
    qmmpaudioengine_p.h \
    audioparameters.h \
    inputsource.h \
    fileinputsource_p.h \
    emptyinputsource_p.h \
    inputsourcefactory.h \
    enginefactory.h \
    metadatamanager.h \
    replaygain_p.h \
    qmmpsettings.h \
    eqsettings.h \
    qmmpevents_p.h \
    volume.h \
    volumehandler.h \
    outputwriter_p.h \
    recycler_p.h \
    qmmpplugincache_p.h \
    channelmap.h \
    channelconverter_p.h \
    audioconverter.h \
    dithering_p.h \
    visualbuffer_p.h \
    lightfactory.h \
    tagmeta.h \
    florid.h \
    qmmp_export.h \
    trackinfo.h \
    cueparser.h \
    qmmptextcodec.h \
    regularexpression.h \
    format.h

SOURCES += recycler.cpp \
    decoder.cpp \
    output.cpp \
    equ/iir.c \
    equ/iir_cfs.c \
    equ/iir_fpu.c \
    soundcore.cpp \
    visual.cpp \
    visualpalette.cpp \
    effect.cpp \
    statehandler.cpp \
    qmmp.cpp \
    volumehandler.cpp \
    metadatamodel.cpp \
    tagmodel.cpp \
    abstractengine.cpp \
    qmmpaudioengine.cpp \
    audioparameters.cpp \
    inputsource.cpp \
    fileinputsource.cpp \
    emptyinputsource.cpp \
    metadatamanager.cpp \
    replaygain.cpp \
    audioconverter.cpp \
    qmmpsettings.cpp \
    eqsettings.cpp \
    qmmpevents.cpp \
    outputwriter.cpp \
    qmmpplugincache.cpp \
    channelmap.cpp \
    channelconverter.cpp \
    volume.cpp \
    dithering.cpp \
    visualbuffer.cpp \
    tagmeta.cpp \
    florid.cpp \
    trackinfo.cpp \
    cueparser.cpp \
    qmmptextcodec.cpp \
    regularexpression.cpp \
    format.cpp

DESTDIR = $$OUT_PWD/../../bin/$$TTK_VERSION

TARGET = TTK$${TARGET}
CONFIG += shared warn_off plugin lib qt thread
TEMPLATE = lib

INCLUDEPATH += \
    $$PWD \
    $$PWD/../../../extra/gcc/libtaglib/include

win32{
    LIBS += -L$$PWD/../../../extra/gcc/libtaglib/lib -ltag
}

unix:!mac{
    LIBS += -L$$PWD/../../../extra/gcc/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}

mac{
    LIBS += -ltag
}
