include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

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

INCLUDEPATH += $$EXTRA_PREFIX/libopenmpt/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libopenmpt/lib -lopenmpt
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libopenmpt/lib -lopenmpt$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lopenmpt
}
