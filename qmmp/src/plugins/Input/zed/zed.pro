TEMPLATE = subdirs

SUBDIRS += adplug
SUBDIRS += archive
SUBDIRS += buzzic
SUBDIRS += dca
SUBDIRS += ken
SUBDIRS += organya
SUBDIRS += psf
SUBDIRS += qoa
SUBDIRS += s98
SUBDIRS += v2m

!mac{
    SUBDIRS += sunvox
    SUBDIRS += xmdx
    SUBDIRS += xsf
}

contains(QT_ARCH, "i386")|contains(QT_ARCH, "x86_64") {
    SUBDIRS += optimfrog
}
