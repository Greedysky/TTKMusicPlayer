include($$PWD/../../plugins.pri)

HEADERS += soxresampler.h \
           effectsoxrfactory.h \
           settingsdialog.h

SOURCES += soxresampler.cpp \
           effectsoxrfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

DESTDIR = $$PLUGINS_PREFIX/Effect
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libsoxr/include

win32:{
    LIBS += -L$$EXTRA_PREFIX/libsoxr/lib -lsoxr
}

unix:{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L/usr/lib -I/usr/include
    LIBS += -L$$EXTRA_PREFIX/libsoxr/lib -lsoxr$$STATIC_LIBRARY_SUFFIX
}
