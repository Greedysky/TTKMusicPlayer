include($$PWD/../../plugins.pri)

HEADERS += decoderopenmptfactory.h \
           decoder_openmpt.h \
           openmptmetadatamodel.h \
           openmpthelper.h \
           archivereader.h \
           settingsdialog.h

SOURCES += decoderopenmptfactory.cpp \
           decoder_openmpt.cpp \
           openmptmetadatamodel.cpp \
           openmpthelper.cpp \
           archivereader.cpp \
           settingsdialog.cpp

FORMS   += settingsdialog.ui

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libopenmpt/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libopenmpt/lib -lopenmpt$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libopenmpt/lib -lopenmpt
}
