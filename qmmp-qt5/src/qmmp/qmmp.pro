include(../../qmmp.pri)

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
    volumecontrol_p.h \
    outputwriter_p.h \
    recycler_p.h \
    qmmpplugincache_p.h \
    channelmap.h \
    channelconverter_p.h \
    audioconverter.h \
    dithering_p.h \
    visualbuffer_p.h \
    lightfactory.h \
    tagreadandwrite.h \
    florid.h \
    qmmp_export.h \
    trackinfo.h

SOURCES += recycler.cpp \
    decoder.cpp \
    output.cpp \
    equ/iir.c \
    equ/iir_cfs.c \
    equ/iir_fpu.c \
    soundcore.cpp \
    visual.cpp \
    effect.cpp \
    statehandler.cpp \
    qmmp.cpp \
    volumecontrol.cpp \
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
    tagreadandwrite.cpp \
    florid.cpp \
    trackinfo.cpp


unix:TARGET = ../../lib/$$TTKMusicPlayer/qmmp
win32:TARGET = ../../../bin/$$TTKMusicPlayer/qmmp

CONFIG += shared warn_on qt thread
TEMPLATE = lib

VERSION = $$QMMP_VERSION

unix {
  isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
  DEFINES += LIB_DIR=\\\"$$LIB_DIR\\\"
}

unix {
    target.path = $$LIB_DIR
    devel.files += \
      buffer.h \
      decoder.h \
      output.h \
      outputfactory.h \
      decoderfactory.h \
      soundcore.h \
      visual.h \
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
      volumecontrol_p.h \
      outputwriter_p.h \
      recycler_p.h \
      qmmpplugincache_p.h \
      channelmap.h \
      channelconverter_p.h \
      audioconverter.h \
      dithering_p.h \
      visualbuffer_p.h \
      lightfactory.h \
      tagreadandwrite.h \
      florid.h \
      qmmp_export.h \
      trackinfo.h

    devel.path = /include/qmmp
    INSTALLS += target devel
    DESTDIR = .
}

INCLUDEPATH += ./ ../../../extra/gcc/libtaglib/include

win32:{
    LIBS += -L../../../extra/gcc/libtaglib/lib -ltag.dll
}

unix {
    unix:android{
        INCLUDEPATH += ../../../extra/android/libtaglib/include
        LIBS += -L../../../extra/android/libtaglib/lib -ltag
    }else{
        LIBS += -L../../../extra/gcc/libtaglib/lib -ltag
    }
    CONFIG += create_pc create_prl no_install_prl
    QMAKE_PKGCONFIG_NAME = qmmp
    QMAKE_PKGCONFIG_DESCRIPTION = qmmp core library
    QMAKE_PKGCONFIG_REQUIRES = Qt5Core Qt5Gui Qt5Widgets
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}
