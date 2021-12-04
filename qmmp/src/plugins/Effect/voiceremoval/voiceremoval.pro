include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

HEADERS += voiceremovalplugin.h \
           effectvoiceremovalfactory.h

SOURCES += voiceremovalplugin.cpp \
           effectvoiceremovalfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
