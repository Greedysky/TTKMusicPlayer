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

INCLUDEPATH += $$EXTRA_LIB_PATH/libenca/include

contains(CONFIG, WITH_ENCA){
    DEFINES += WITH_ENCA
    win32:LIBS += -L$$EXTRA_LIB_PATH/libenca/lib -lenca
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libenca/lib -lenca$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lenca
}
