CONFIG += ordered

include(qmmp.pri)

TEMPLATE = subdirs
SUBDIRS += src/qmmp src/plugins

contains(CONFIG, BUILD_DEMO){
    SUBDIRS += src/demo
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
    exists($$EXTRA_DEPENDENT_PATH){
        EXTRA_OUTPUT_PATH = $$PWD/../extra/gcc
        EXTRA_OUTPUT_PATH = $$replace(EXTRA_OUTPUT_PATH, /, \\)
        EXTRA_DEPENDENT_PATH = $$replace(EXTRA_DEPENDENT_PATH, /, \\)
        system(rd $$EXTRA_OUTPUT_PATH) #Remove old one
        system(mklink /j $$EXTRA_OUTPUT_PATH $$EXTRA_DEPENDENT_PATH) #Link new one
    }
}
