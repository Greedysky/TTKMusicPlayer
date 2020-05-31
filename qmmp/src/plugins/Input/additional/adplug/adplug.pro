include($$PWD/../additional.pri)

HEADERS += decoderadplugfactory.h \
           decoder_adplug.h \
           adplughelper.h \
           adplugmetadatamodel.h \
           magic.h

SOURCES += decoderadplugfactory.cpp \
           decoder_adplug.cpp \
           adplughelper.cpp \
           adplugmetadatamodel.cpp \
           magic.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libadplug/include \
               $$EXTRA_PREFIX/libadplug/include/libbinio

unix:{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libadplug/lib -ladplug$$STATIC_LIBRARY_SUFFIX -lbinio$$STATIC_LIBRARY_SUFFIX
}

win32:{
    LIBS += -L$$EXTRA_PREFIX/libadplug/lib -ladplug -lbinio
}
