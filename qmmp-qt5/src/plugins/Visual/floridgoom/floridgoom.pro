include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/floridgoom
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libfloridgoom.so

HEADERS += goomwidget.h \
           visualfloridgoomfactory.h \
           cpu_info.h \
           default_scripts.h \
           drawmethods.h \
           gfontlib.h \
           goom.h \
           goom_config.h \
           goom_config_param.h \
           goom_filters.h \
           goom_fx.h \
           goom_graphic.h \
           goom_plugin_info.h \
           goomsl.h \
           goomsl_hash.h \
           goomsl_heap.h \
           goomsl_private.h \
           goomsl_yacc.h \
           goom_tools.h \
           goom_typedefs.h \
           goom_visual_fx.h \
           ifs.h \
           lines.h \
           mathtools.h \
           mmx.h \
           motif_goom1.h \
           motif_goom2.h \
           sound_tester.h \
           surf3d.h \
           tentacle3d.h \
           v3d.h \
           attributes.h

SOURCES += goomwidget.cpp \
           visualfloridgoomfactory.cpp \
           config_param.c \
           convolve_fx.c \
           cpu_info.c \
           drawmethods.c \
           filters.c \
           flying_stars_fx.c \
           gfontlib.c \
           gfontrle.c \
           goom_core.c \
           goomsl.c \
           goomsl_hash.c \
           goomsl_heap.c \
           goomsl_lex.c \
           goomsl_yacc.c \
           goom_tools.c \
           graphic.c \
           ifs.c \
           lines.c \
           mathtools.c \
           plugin_info.c \
           sound_tester.c \
           surf3d.c \
           tentacle3d.c \
           v3d.c

DEFINES += YY_NO_INPUT

contains(QT_ARCH, i386){
    DEFINES += CPU_X86 ARCH_X86_32
    message(Architecture is x86)
    HEADERS += mmx.h
    SOURCES += mmx.c xmmx.c
}
contains(QT_ARCH, x86_64){
    #unsupported
    DEFINES += CPU_X86 ARCH_X86_64
    message(Architecture is x86_64)
    HEADERS += mmx.h
    SOURCES += mmx.c xmmx.c
}

QMAKE_CFLAGS += -Werror=implicit-function-declaration

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
