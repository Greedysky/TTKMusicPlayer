include(../../plugins.pri)
FORMS += settingsdialog.ui
HEADERS += decodercuefactory.h \
    cueparser.h \
    decoder_cue.h \
    settingsdialog.h \
    cuemetadatamodel.h
SOURCES += decoder_cue.cpp \
    decodercuefactory.cpp \
    cueparser.cpp \
    settingsdialog.cpp \
    cuemetadatamodel.cpp
win32:HEADERS += ../../../../src/qmmp/decoder.h \
    ../../../../src/qmmp/statehandler.h
TARGET = $$PLUGINS_PREFIX/Input/cue
unix:QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libcue.so
INCLUDEPATH += ../../../
CONFIG += warn_on \
    plugin
TEMPLATE = lib
unix{
   QMAKE_LIBDIR += ../../../../lib
   LIBS += -lqmmp -L/usr/lib
}

win32{
   QMAKE_LIBDIR += ../../../../bin
   LIBS += -lqmmp0
}

contains(CONFIG, WITH_ENCA){
   CONFIG += link_pkgconfig
   DEFINES += WITH_ENCA
   unix:PKGCONFIG += enca
   win32:{
      gcc{
          INCLUDEPATH += $$EXTRA_PREFIX/libenca/include
          LIBS += -L$$EXTRA_PREFIX/libenca/lib -lenca
      }
   }

}


unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
}
