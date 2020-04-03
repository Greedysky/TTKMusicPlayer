include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decoderpsffactory.h \
           decoder_psf.h \
           psfhelper.h \
           psfmetadatamodel.h \
           ao.h \
           corlett.h \
           cpuintrf.h \
           eng_protos.h \
           mem.h \
           osd_cpu.h \
           eng_dsf/aicadsp.h \
           eng_dsf/aica.h \
           eng_dsf/arm7.h \
           eng_dsf/arm7i.h \
           eng_dsf/arm7thumb.h \
           eng_dsf/dc_hw.h \
           eng_ssf/m68kconf.h \
           eng_ssf/m68kcpu.h \
           eng_ssf/m68k.h \
           eng_ssf/m68kmame.h \
           eng_ssf/m68kops.h \
           eng_ssf/sat_hw.h \
           eng_ssf/scspdsp.h \
           eng_ssf/scsp.h \
           eng_qsf/qsound.h \
           eng_qsf/z80dasm.h \
           eng_qsf/z80.h \
           eng_psf/cpuintrf.h \
           eng_psf/mamemem.h \
           eng_psf/psx.h \
           eng_psf/peops/adsr.h \
           eng_psf/peops/dma.h \
           eng_psf/peops/externals.h \
           eng_psf/peops/gauss_i.h \
           eng_psf/peops/registers.h \
           eng_psf/peops/regs.h \
           eng_psf/peops/spu.h \
           eng_psf/peops/stdafx.h \
           eng_psf/peops2/adsr.h \
           eng_psf/peops2/dma.h \
           eng_psf/peops2/externals.h \
           eng_psf/peops2/gauss_i.h \
           eng_psf/peops2/psemuxa.h \
           eng_psf/peops2/registers.h \
           eng_psf/peops2/regs.h \
           eng_psf/peops2/reverb.h \
           eng_psf/peops2/spu.h \
           eng_psf/peops2/stdafx.h
    
SOURCES += decoderpsffactory.cpp \
           decoder_psf.cpp \
           psfhelper.cpp \
           psfmetadatamodel.cpp \
           corlett.c \
           ao.c \
           eng_dsf/eng_dsf.c \
           eng_dsf/dc_hw.c \
           eng_dsf/aica.c \
           eng_dsf/aicadsp.c \
           eng_dsf/arm7.c \
           eng_dsf/arm7i.c \
           eng_ssf/m68kcpu.c \
           eng_ssf/m68kopac.c \
           eng_ssf/m68kopdm.c \
           eng_ssf/m68kopnz.c \
           eng_ssf/m68kops.c \
           eng_ssf/scsp.c \
           eng_ssf/scspdsp.c \
           eng_ssf/sat_hw.c \
           eng_ssf/eng_ssf.c \
           eng_qsf/eng_qsf.c \
           eng_qsf/kabuki.c \
           eng_qsf/qsound.c \
           eng_qsf/z80.c \
           eng_qsf/z80dasm.c \
           eng_psf/eng_psf.c \
           eng_psf/psx.c \
           eng_psf/psx_hw.c \
           eng_psf/peops/spu.c \
           eng_psf/eng_psf2.c \
           eng_psf/peops2/spu2.c \
           eng_psf/peops2/dma2.c \
           eng_psf/peops2/registers2.c \
           eng_psf/eng_spu.c

HEADERS += zlib/zconf.h \
           zlib/zlib.h \
           zlib/crc32.h \
           zlib/deflate.h \
           zlib/gzguts.h \
           zlib/inffixed.h \
           zlib/inffast.h \
           zlib/inflate.h \
           zlib/inftrees.h \
           zlib/trees.h \
           zlib/zutil.h

SOURCES += zlib/adler32.c \
           zlib/compress.c \
           zlib/crc32.c \
           zlib/deflate.c \
           zlib/gzclose.c \
           zlib/gzlib.c \
           zlib/gzread.c \
           zlib/gzwrite.c \
           zlib/inflate.c \
           zlib/infback.c \
           zlib/inftrees.c \
           zlib/inffast.c \
           zlib/trees.c \
           zlib/uncompr.c \
           zlib/zutil.c

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = psf

unix{
    unix:android {
        TARGET = plugin_input_psf
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libplugin_input_psf.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libpsf.so
    }
}
