include(../../plugins.pri)

HEADERS += decodercdaudiofactory.h \
           decoder_cdaudio.h \

SOURCES += decoder_cdaudio.cpp \
           decodercdaudiofactory.cpp \

INCLUDEPATH += $$EXTRA_PREFIX/libcddb/include \
               $$EXTRA_PREFIX/libcdio/include \
               $$EXTRA_PREFIX/libcdio_paranoia/include


TARGET =$$PLUGINS_PREFIX/Input/cdaudio
QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libcdaudio.so

unix {
    LIBS += -L$$EXTRA_PREFIX/libcdio/lib -lcdio \
            -L$$EXTRA_PREFIX/libcdio_paranoia/lib -lcdio_paranoia -lcdio_cdda
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
}

win32 {
  HEADERS += ../../../../src/qmmp/metadatamodel.h \
             ../../../../src/qmmp/decoderfactory.h
  LIBS += -L$$EXTRA_PREFIX/libcdio/lib -lcdio \
          -L$$EXTRA_PREFIX/libcdio_paranoia/lib -lcdio_paranoia -lcdio_cdda \
          -L$$EXTRA_PREFIX/libcddb/lib -lcddb \
          -lm -lwinmm -mwindows -liconv -lws2_32
}