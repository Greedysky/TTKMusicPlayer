include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decoderdumbfactory.h \
           decoder_dumb.h \
           dumbhelper.h \
           dumbmetadatamodel.h \
           internal/barray.h \
           internal/dumb.h \
           internal/dumbfile.h \
           internal/it.h \
           internal/lpc.h \
           internal/resampler.h \
           internal/riff.h \
           internal/stack_alloc.h \
           internal/tarray.h \
           tools/it/modulus.h \
           dumb.h \
           modloader.h \
           umr.h \
           unmo3.h \
           urf.h

SOURCES += decoderdumbfactory.cpp \
           decoder_dumb.cpp \
           dumbhelper.cpp \
           dumbmetadatamodel.cpp \
           modloader.cpp \
           unrealfmt.cpp \
           unrealfmtdata.cpp \
           it/loadmod2.c \
           it/itorder.c \
           it/readokt2.c \
           it/readptm.c \
           it/loadxm.c \
           it/ptmeffect.c \
           it/loadany2.c \
           it/loadokt2.c \
           it/loadasy.c \
           it/readasy.c \
           it/loadpsm2.c \
           it/readstm.c \
           it/readxm2.c \
           it/readmod.c \
           it/readam.c \
           it/loadoldpsm2.c \
           it/loadxm2.c \
           it/loadmod.c \
           it/loadany.c \
           it/loadmtm.c \
           it/itrender.c \
           it/loadasy2.c \
           it/readpsm.c \
           it/itload.c \
           it/loadriff2.c \
           it/itread.c \
           it/loadmtm2.c \
           it/loadriff.c \
           it/readmtm.c \
           it/reads3m.c \
           it/itload2.c \
           it/readxm.c \
           it/loadpsm.c \
           it/readany2.c \
           it/loadamf.c \
           it/loadptm2.c \
           it/readokt.c \
           it/itread2.c \
           it/itmisc.c \
           it/loadokt.c \
           it/loads3m.c \
           it/loadptm.c \
           it/readdsmf.c \
           it/readamf2.c \
           it/itunload.c \
           it/readmod2.c \
           it/readamf.c \
           it/readoldpsm.c \
           it/loadamf2.c \
           it/read6692.c \
           it/read669.c \
           it/readriff.c \
           it/readany.c \
           it/load669.c \
           it/loadstm2.c \
           it/loadstm.c \
           it/load6692.c \
           it/readstm2.c \
           it/reads3m2.c \
           it/loads3m2.c \
           it/loadoldpsm.c \
           it/xmeffect.c \
           helpers/riff.c \
           helpers/memfile.c \
           helpers/silence.c \
           helpers/stdfile.c \
           helpers/clickrem.c \
           helpers/tarray.c \
           helpers/resample.c \
           helpers/barray.c \
           helpers/lpc.c \
           helpers/resampler.c \
           helpers/sampbuf.c \
           core/unload.c \
           core/loadduh.c \
           core/duhlen.c \
           core/register.c \
           core/rawsig.c \
           core/rendduh.c \
           core/makeduh.c \
           core/duhtag.c \
           core/readduh.c \
           core/dumbfile.c \
           core/atexit.c \
           core/rendsig.c \
           helpers/resample.inc \
           helpers/resamp2.inc \
           helpers/resamp3.inc

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = dumb

# Disbale gcc warnings
QMAKE_CFLAGS += -w
QMAKE_CXXFLAGS += -w

INCLUDEPATH += $$PWD

unix{
    QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libdumb.so
}
