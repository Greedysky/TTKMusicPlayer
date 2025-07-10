include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderadplugfactory.h \
           decoder_adplug.h \
           adplughelper.h \
           adplugmetadatamodel.h \
           settingsdialog.h

SOURCES += decoderadplugfactory.cpp \
           decoder_adplug.cpp \
           adplughelper.cpp \
           adplugmetadatamodel.cpp \
           settingsdialog.cpp

FORMS   += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libadplug/include \
               $$EXTRA_PREFIX/libadplug/include/libbinio

win32{
    LIBS += -L$$EXTRA_PREFIX/libadplug/lib -ladplug -lbinio
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libadplug/lib -ladplug$$STATIC_LIBRARY_SUFFIX -lbinio$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -ladplug -lbinio
}
