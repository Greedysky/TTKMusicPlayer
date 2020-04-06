include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decodersc68factory.h \
           decoder_sc68.h \
           sc68helper.h \
           sc68metadatamodel.h \
           file68/src/replay.inc.h \
           file68/src/timedb.inc.h \
           file68/sc68/file68_vfs_ao.h \
           file68/sc68/file68.h \
           file68/sc68/file68_ord.h \
           file68/sc68/file68_tag.h \
           file68/sc68/file68_vfs_curl.h \
           file68/sc68/file68_str.h \
           file68/sc68/file68_features.h \
           file68/sc68/file68_vfs_def.h \
           file68/sc68/file68_msg.h \
           file68/sc68/file68_vfs_file.h \
           file68/sc68/file68_api.h \
           file68/sc68/file68_chk.h \
           file68/sc68/file68_ice.h \
           file68/sc68/file68_err.h \
           file68/sc68/file68_opt.h \
           file68/sc68/file68_vfs_mem.h \
           file68/sc68/file68_rsc.h \
           file68/sc68/file68_zip.h \
           file68/sc68/file68_vfs_null.h \
           file68/sc68/file68_tdb.h \
           file68/sc68/file68_reg.h \
           file68/sc68/file68_vfs_fd.h \
           file68/sc68/file68_vfs.h \
           file68/sc68/file68_uri.h \
           file68/sc68/file68_vfs_z.h \
           libsc68/dial68/dial68.h \
           unice68/unice68.h \
           libsc68/io68/io68_api.h \
           libsc68/io68/io68.h \
           libsc68/io68/shifter_io.h \
           libsc68/io68/mfp_io.h \
           libsc68/io68/ym_dump.h \
           libsc68/io68/ym_blep.h \
           libsc68/io68/mwemul.h \
           libsc68/io68/ymoutorg.h \
           libsc68/io68/mw_io.h \
           libsc68/io68/mfpemul.h \
           libsc68/io68/paula_io.h \
           libsc68/io68/paulaemul.h \
           libsc68/io68/ym_fixed_vol.h \
           libsc68/io68/ym_puls.h \
           libsc68/io68/ymemul.h \
           libsc68/io68/ymout2k9.h \
           libsc68/io68/default.h \
           libsc68/io68/ym_io.h \
           libsc68/emu68/inl68_datamove.h \
           libsc68/emu68/lines68.h \
           libsc68/emu68/inl68_bitmanip.h \
           libsc68/emu68/inst68.h \
           libsc68/emu68/inl68_progctrl.h \
           libsc68/emu68/cc68.h \
           libsc68/emu68/macro68.h \
           libsc68/emu68/mem68.h \
           libsc68/emu68/error68.h \
           libsc68/emu68/inl68_logic.h \
           libsc68/emu68/assert68.h \
           libsc68/emu68/inl68_exception.h \
           libsc68/emu68/srdef68.h \
           libsc68/emu68/type68.h \
           libsc68/emu68/struct68.h \
           libsc68/emu68/excep68.h \
           libsc68/emu68/emu68.h \
           libsc68/emu68/inl68_shifting.h \
           libsc68/emu68/inl68_arithmetic.h \
           libsc68/emu68/inl68_bcd.h \
           libsc68/emu68/inl68_systctrl.h \
           libsc68/emu68/emu68_api.h \
           libsc68/emu68/ioplug68.h \
           libsc68/sc68/trap68.h \
           libsc68/sc68/conf68.h \
           libsc68/sc68/sc68.h \
           libsc68/sc68/mixer68.h \
           desa68/desa68.h
    
SOURCES += decodersc68factory.cpp \
           decoder_sc68.cpp \
           sc68helper.cpp \
           sc68metadatamodel.cpp \
           file68/src/gzip68.c \
           file68/src/vfs68.c \
           file68/src/ice68.c \
           file68/src/vfs68_ao.c \
           file68/src/vfs68_null.c \
           file68/src/msg68.c \
           file68/src/timedb68.c \
           file68/src/init68.c \
           file68/src/vfs68_z.c \
           file68/src/replay68.c \
           file68/src/registry68.c \
           file68/src/uri68.c \
           file68/src/rsc68.c \
           file68/src/vfs68_fd.c \
           file68/src/vfs68_file.c \
           file68/src/vfs68_mem.c \
           file68/src/file68.c \
           file68/src/string68.c \
           file68/src/endian68.c \
           file68/src/vfs68_curl.c \
           file68/src/option68.c \
           file68/src/ferror68.c \
           libsc68/dial68/dial68.c \
           libsc68/dial68/dial_conf.c \
           libsc68/dial68/dial_finf.c \
           libsc68/dial68/dial_tsel.c \
           unice68/unice68_unpack.c \
           unice68/unice68_pack.c \
           unice68/unice68_version.c \
           libsc68/conf68.c \
           libsc68/api68.c \
           libsc68/mixer68.c \
           libsc68/io68/paulaemul.c \
           libsc68/io68/ym_envel.c \
           libsc68/io68/ymemul.c \
           libsc68/io68/shifter_io.c \
           libsc68/io68/paula_io.c \
           libsc68/io68/mw_io.c \
           libsc68/io68/ym_dump.c \
           libsc68/io68/mfp_io.c \
           libsc68/io68/mwemul.c \
           libsc68/io68/ym_io.c \
           libsc68/io68/io68.c \
           libsc68/io68/ym_blep.c \
           libsc68/io68/mfpemul.c \
           libsc68/io68/ym_puls.c \
           libsc68/libsc68.c \
           libsc68/emu68/lines68.c \
           libsc68/emu68/ioplug68.c \
           libsc68/emu68/mem68.c \
           libsc68/emu68/getea68.c \
           libsc68/emu68/inst68.c \
           libsc68/emu68/emu68.c \
           libsc68/emu68/error68.c \
           desa68/desa68.c

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = sc68

INCLUDEPATH += $$PWD \
               $$PWD/file68 \
               $$PWD/file68/src \
               $$PWD/file68/sc68 \
               $$PWD/libsc68 \
               $$PWD/libsc68/dial68 \
               $$PWD/libsc68/emu68 \
               $$PWD/libsc68/io68 \
               $$PWD/libsc68/sc68 \
               $$PWD/unice68 \
               $$PWD/desa68

DEFINES += HAVE_STDLIB_H \
           HAVE_STDINT_H \
           HAVE_STRING_H \
           HAVE_LIMITS_H \
           HAVE_ASSERT_H \
           HAVE_BASENAME \
           EMU68_MONOLITIC \
           EMU68_EXPORT

unix{
    QMAKE_CLEAN = $$DESTDIR/libsc68.so
}
