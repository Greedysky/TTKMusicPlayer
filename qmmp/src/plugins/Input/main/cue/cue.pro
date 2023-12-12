include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodercuefactory.h \
           decoder_cue.h \
           cuefile.h \
           cuemetadatamodel.h \
           settingsdialog.h

SOURCES += decodercuefactory.cpp \
           decoder_cue.cpp \
           cuefile.cpp \
           cuemetadatamodel.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libenca/include

unix{
   QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
   LIBS += -L$$EXTRA_PREFIX/libenca/lib -lenca$$STATIC_LIBRARY_SUFFIX
}

contains(CONFIG, WITH_ENCA){
   DEFINES += WITH_ENCA
   win32:LIBS += -L$$EXTRA_PREFIX/libenca/lib -lenca
}
