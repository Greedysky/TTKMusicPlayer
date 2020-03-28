include($$PWD/../../plugins.pri)

HEADERS += decoderffapfactory.h \
           ffap.h \
           decoder_ffap.h \
           cueparser.h \
           ffapmetadatamodel.h \
           decoder_ffapcue.h

SOURCES += decoderffapfactory.cpp \
           ffap.c \
           decoder_ffap.cpp \
           cueparser.cpp \
           ffapmetadatamodel.cpp \
           decoder_ffapcue.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = ffap

INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include

#contains(QT_ARCH, i386)|contains(QT_ARCH, x86_64)|contains(QT_ARCH, windows) {
#    ASM_SOURCES += dsputil_yasm.asm x86inc.asm
#    asm_compiler.commands = yasm
#    asm_compiler.commands += ${QMAKE_FILE_IN} -o ${QMAKE_FILE_OUT}
#    asm_compiler.output = ${QMAKE_VAR_OBJECTS_DIR}${QMAKE_FILE_BASE}$${first(QMAKE_EXT_OBJ)}
#    asm_compiler.input = ASM_SOURCES
#    asm_compiler.variable_out = OBJECTS
#    asm_compiler.name = compiling[asm] ${QMAKE_FILE_IN}
#    silent:asm_compiler.commands = @echo compiling[asm] ${QMAKE_FILE_IN} && $$asm_compiler.commands

#    QMAKE_EXTRA_COMPILERS += asm_compiler
#    DEFINES += HAVE_SSE2

#    contains(QT_ARCH, i386) {
#        message(Architecture is x86)
#        DEFINES += ARCH_X86_32
#        asm_compiler.commands += -f elf -m x86 -DARCH_X86_32
#    } else:contains(QT_ARCH, x86_64) {
#        message(Architecture is x86_64)
#        DEFINES += ARCH_X86_64
#        asm_compiler.commands += -f elf64 -m amd64 -DARCH_X86_64 -DPIC
#    } else:contains(QT_ARCH, windows) {
#        message(Architecture is x86)
#        DEFINES += ARCH_X86_32
#        asm_compiler.commands += -f win32 -DPREFIX
#    }
#}else{
#    message(Architecture is unknown)
#    DEFINES += ARCH_UNKNOWN
#}

unix {
    QMAKE_CLEAN = $$DESTDIR/libffap.so
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll
}
