include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

HEADERS += soxresampler.h \
           effectsoxrfactory.h \
           settingsdialog.h

SOURCES += soxresampler.cpp \
           effectsoxrfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libsoxr/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libsoxr/lib -lsoxr
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libsoxr/lib -lsoxr$$STATIC_LIBRARY_SUFFIX
}
