#Common settings for Qmmp build

#Extra clean target

unix:QMAKE_DISTCLEAN += -r .build

#Some conf to redirect intermediate stuff in separate dirs

UI_DIR=./.build/ui/
MOC_DIR=./.build/moc/
OBJECTS_DIR=./.build/obj
RCC_DIR=./.build/rcc

#Defines

DEFINES += QT_NO_CAST_FROM_BYTEARRAY QT_STRICT_ITERATORS

greaterThan(QT_MAJOR_VERSION, 4){
CONFIG -= depend_includepath
QT += widgets
unix:DEFINES += QMMP_WS_X11
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050400 QT_DEPRECATED_WARNINGS
}

#Configuration

CONFIG += hide_symbols
CONFIG += c++11
QMAKE_CFLAGS += -std=gnu11
greaterThan(QT_MAJOR_VERSION, 5){
    QMAKE_CXXFLAGS += -std=c++17
}else{
    QMAKE_CXXFLAGS += -std=c++11
}
DEFINES += QMMP_LIBRARY


#Version
TTK_VERSION = 4.2.0.0
QMMP_VERSION = 1.7.8

CONFIG += BUILD_DEMO
CONFIG += USE_STATIC_LIBRARY

#Comment/uncomment this if you want to change plugins list

CONFIG += BITCRUSHER_PLUGIN
CONFIG += BS2B_PLUGIN
CONFIG += CRYSTALIZER_PLUGIN
CONFIG += ECHO_PLUGIN
CONFIG += LADSPA_PLUGIN
CONFIG += MUFFLER_PLUGIN
CONFIG += SILENCE_PLUGIN
CONFIG += SONIC_PLUGIN
CONFIG += SOXR_PLUGIN
CONFIG += SRC_PLUGIN
CONFIG += SUBWOOFER_PLUGIN

CONFIG += AAC_PLUGIN
CONFIG += ADLMIDI_PLUGIN
CONFIG += CDAUDIO_PLUGIN
CONFIG += FFAP_PLUGIN
CONFIG += FFMPEG_PLUGIN
CONFIG += FLAC_PLUGIN
CONFIG += GME_PLUGIN
CONFIG += MUSEPACK_PLUGIN
CONFIG += WITH_LIBCDDB
CONFIG += WITH_ENCA
CONFIG += OPUS_PLUGIN
CONFIG += SID_PLUGIN

#Tracker features
CONFIG += ASAP_PLUGIN
CONFIG += AYFLY_PLUGIN
CONFIG += FC14_PLUGIN
CONFIG += HIVELY_PLUGIN
CONFIG += SC68_PLUGIN
CONFIG += SOUNDMON_PLUGIN
CONFIG += STSOUND_PLUGIN
CONFIG += TFMX_PLUGIN
CONFIG += XMP_PLUGIN
CONFIG += UADE_PLUGIN
#CONFIG += VGMSTREAM_PLUGIN

#Zed features
CONFIG += ARCHIVE_PLUGIN
CONFIG += BUZZIC_PLUGIN
CONFIG += DCA_PLUGIN
CONFIG += KEN_PLUGIN
CONFIG += ORGANYA_PLUGIN
CONFIG += PSF_PLUGIN
CONFIG += QOA_PLUGIN
CONFIG += S98_PLUGIN
CONFIG += V2M_PLUGIN
CONFIG += SUNVOX_PLUGIN
CONFIG += XMDX_PLUGIN
CONFIG += XSF_PLUGIN
CONFIG += OPTIMFROG_PLUGIN

CONFIG -= $$DISABLED_PLUGINS

contains(CONFIG, USE_STATIC_LIBRARY){
    STATIC_LIB_SUFFIX = _static
    DEFINES += TAGLIB_STATIC
}else{
    STATIC_LIB_SUFFIX =
}

win32{
    equals(QT_MAJOR_VERSION, 6){ #Qt6
        greaterThan(QT_MINOR_VERSION, 7){ #6.8
            GCC_VERSION = 1310
        }else:greaterThan(QT_MINOR_VERSION, 1){ #6.2
            GCC_VERSION = 1120
        }else{
            GCC_VERSION = 810
        }
    }else:equals(QT_MAJOR_VERSION, 5){ #Qt5
        greaterThan(QT_MINOR_VERSION, 14){ #5.15
            GCC_VERSION = 810
        }else:greaterThan(QT_MINOR_VERSION, 11){ #5.12
            GCC_VERSION = 730
        }else:greaterThan(QT_MINOR_VERSION, 6){ #5.7
            GCC_VERSION = 530
        }else{
            GCC_VERSION = 492
        }
    }else{
        GCC_VERSION = 492
    }

    contains(QT_ARCH, "x86_64"){
        GCC_ARCH = 64
    }else{#contains(QT_ARCH, "i386")
        GCC_ARCH = 32
    }

    EXTRA_DEPENDENT_PATH = $$PWD/../extra/gcc$${GCC_VERSION}_$${GCC_ARCH}
    message("Extra thirdParty library path: $$EXTRA_DEPENDENT_PATH")
    exists($$EXTRA_DEPENDENT_PATH){
        system(rd $$PWD/../extra/gcc) #Remove old one
        system(mklink /D $$PWD/../extra/gcc $$EXTRA_DEPENDENT_PATH) #Link new one
    }
}
#Add dependent thirdParty library path
EXTRA_LIB_PATH = $$PWD/../extra/gcc
