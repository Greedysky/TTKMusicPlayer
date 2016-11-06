include(../../plugins.pri)

TARGET =$$PLUGINS_PREFIX/Visual/projectm
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libprojectm.so

HEADERS += projectmwidget.h \
           visualprojectmfactory.h \
           projectmplugin.h \
           projectmwrapper.h

SOURCES += projectmplugin.cpp \
           visualprojectmfactory.cpp \
           projectmwidget.cpp \
           projectmwrapper.cpp


INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libprojectM/include

CONFIG += warn_on \
plugin \
link_pkgconfig

TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
QT += opengl

unix {
  isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
  target.path = $$LIB_DIR/qmmp/Visual
  INSTALLS += target
  LIBS += -L$$EXTRA_PREFIX/libprojectM/lib -lprojectM -lqmmp -L/usr/lib -I/usr/include
  #projectM config path
  PROJECTM_CONFIG_FILES = $$EXTRA_PREFIX/libprojectM/share/projectM/config.inp \
                          $$EXTRA_PREFIX/libprojectM/share/projectM/config.inp
  for(path, PROJECTM_CONFIG_FILES) {
    exists($$path) {
      message ("found projectm configuration: "$$path)
      DEFINES += PROJECTM_CONFIG=\\\"$$path\\\"
    }
  }
}

win32 {
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libprojectM/lib -lglew32 -lopengl32 -lprojectM.dll -lqmmp1
}
