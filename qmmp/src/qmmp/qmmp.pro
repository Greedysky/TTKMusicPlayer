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
    tagreadandwrite.h \
    florid.h \
    qmmp_export.h \
    trackinfo.h \
    cueparser.h

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
    tagreadandwrite.cpp \
    florid.cpp \
    trackinfo.cpp \
    cueparser.cpp

DESTDIR = $$OUT_PWD/../../bin/$$TTKMusicPlayer

TARGET = $${TARGET}
CONFIG += shared warn_on qt thread
TEMPLATE = lib

VERSION = $$QMMP_VERSION

INCLUDEPATH += $$PWD/ $$PWD/../../../extra/gcc/libtaglib/include

win32:{
    LIBS += -L$$PWD/../../../extra/gcc/libtaglib/lib -ltag
}

unix {
    LIBS += -L$$PWD/../../../extra/gcc/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
    CONFIG += create_pc create_prl no_install_prl
    QMAKE_PKGCONFIG_NAME = qmmp
    QMAKE_PKGCONFIG_DESCRIPTION = qmmp core library
    equals(QT_MAJOR_VERSION, 4): QMAKE_PKGCONFIG_REQUIRES = QtCore QtGui
    equals(QT_MAJOR_VERSION, 5): QMAKE_PKGCONFIG_REQUIRES = Qt5Core Qt5Gui Qt5Widgets
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}
