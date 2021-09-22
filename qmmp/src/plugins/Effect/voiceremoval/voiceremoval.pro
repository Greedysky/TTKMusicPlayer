include($$PWD/../../plugins.pri)

HEADERS += voiceremovalplugin.h \
           effectvoiceremovalfactory.h

SOURCES += voiceremovalplugin.cpp \
           effectvoiceremovalfactory.cpp

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
