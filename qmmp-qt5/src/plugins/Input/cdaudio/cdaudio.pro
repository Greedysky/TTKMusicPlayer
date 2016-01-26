include(../../plugins.pri)

HEADERS += decodercdaudiofactory.h \
           decoder_cdaudio.h \
           settingsdialog.h
SOURCES += decoder_cdaudio.cpp \
           decodercdaudiofactory.cpp \
           settingsdialog.cpp


INCLUDEPATH += ../../../
CONFIG += warn_on \
plugin \
link_pkgconfig
TEMPLATE = lib

TARGET =$$PLUGINS_PREFIX/Input/cdaudio

RESOURCES = translations/translations.qrc


FORMS += settingsdialog.ui

unix {
  QMAKE_LIBDIR += ../../../../lib
  LIBS += -lqmmp -L/usr/lib  -I/usr/include
  PKGCONFIG += libcdio libcdio_cdda libcdio_paranoia libcddb
  isEmpty(LIB_DIR) {
    LIB_DIR = /lib
  }
  target.path = $$LIB_DIR/qmmp/Input
  QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libcdaudio.so
  INSTALLS += target
}

win32 {
  HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
  QMAKE_LIBDIR += ../../../../bin

  gcc{
      INCLUDEPATH += D:/Qt/Workspace/qmmp_all/gcc/libcdio/include \
                     D:/Qt/Workspace/qmmp_all/gcc/libcdio_paranoia/include
      LIBS += -LD:/Qt/Workspace/qmmp_all/gcc/libcdio/lib -lcdio \
              -LD:/Qt/Workspace/qmmp_all/gcc/libcdio_paranoia/lib -lcdio_paranoia -lcdio_cdda \
              -lqmmp1 -lm -lwinmm -mwindows -liconv -lcddb -lws2_32
  }
#  LIBS += -lqmmp0 -lcdio -lcdio_paranoia -lcdio_cdda  -lm -lwinmm -mwindows -liconv -lcddb -lws2_32
}
