include(../../plugins.pri)

HEADERS += ladspahost.h \
           effectladspafactory.h \
           settingsdialog.h \
           ladspaslider.h \
           ladspa.h \
           ladspahelper.h \
           ladspabutton.h
    
SOURCES += ladspahost.cpp \
           effectladspafactory.cpp \
           settingsdialog.cpp \
           ladspaslider.cpp \
           ladspahelper.cpp \
           ladspabutton.cpp
    
TARGET = $$PLUGINS_PREFIX/Effect/ladspa
QMAKE_CLEAN = $$PLUGINS_PREFIX/Effect/libladspa.so

INCLUDEPATH += ../../../

CONFIG += warn_on \
          plugin \
          link_pkgconfig
    
TEMPLATE = lib

QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer

LIBS += -lqmmp \
        -L/usr/lib \
        -I/usr/include

linux-g++|linux-g++-32|linux-g++-64:LIBS += -ldl

isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
target.path = $$LIB_DIR/qmmp/Effect
INSTALLS += target
FORMS += settingsdialog.ui
