include($$PWD/../../plugins.pri)

TEMPLATE = subdirs

contains(CONFIG, ADPLUG_PLUGIN){
    SUBDIRS += adplug
}

contains(CONFIG, ARCHIVE_PLUGIN){
    SUBDIRS += archive
}

contains(CONFIG, BUZZIC_PLUGIN){
    SUBDIRS += buzzic
}

contains(CONFIG, DCA_PLUGIN){
    SUBDIRS += dca
}

contains(CONFIG, KEN_PLUGIN){
    SUBDIRS += ken
}

contains(CONFIG, ORGANYA_PLUGIN){
    SUBDIRS += organya
}

contains(CONFIG, PSF_PLUGIN){
    SUBDIRS += psf
}

contains(CONFIG, QOA_PLUGIN){
    SUBDIRS += qoa
}

contains(CONFIG, S98_PLUGIN){
    SUBDIRS += s98
}

contains(CONFIG, V2M_PLUGIN){
    SUBDIRS += v2m
}

!mac{
    contains(CONFIG, SUNVOX_PLUGIN){
        SUBDIRS += sunvox
    }

    contains(CONFIG, XMDX_PLUGIN){
        SUBDIRS += xmdx
    }

    contains(CONFIG, XSF_PLUGIN){
        SUBDIRS += xsf
    }
}

contains(QT_ARCH, "i386")|contains(QT_ARCH, "x86_64"){
    contains(CONFIG, OPTIMFROG_PLUGIN){
        SUBDIRS += optimfrog
    }
}
