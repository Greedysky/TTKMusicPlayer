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
SUBDIRS += sunvox
SUBDIRS += v2m
SUBDIRS += xmdx
SUBDIRS += xsf

contains(QT_ARCH, "i386")|contains(QT_ARCH, "x86_64") {
    SUBDIRS += optimfrog
}
