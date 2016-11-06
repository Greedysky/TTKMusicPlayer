include(../../plugins.pri)

HEADERS += outputalsa.h \
           outputalsafactory.h  \

SOURCES += outputalsa.cpp \
           outputalsafactory.cpp  \


TARGET=$$PLUGINS_PREFIX/Output/alsa
QMAKE_CLEAN =$$PLUGINS_PREFIX/Output/libalsa.so

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libalsa/include
QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer

CONFIG += warn_on \
          thread \
          plugin \
          link_pkgconfig

TEMPLATE = lib
LIBS += -L$$EXTRA_PREFIX/libalsa/lib -lasound -lqmmp

isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
