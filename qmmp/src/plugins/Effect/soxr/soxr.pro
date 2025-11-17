include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Effect

HEADERS += soxresampler.h \
           effectsoxrfactory.h \
           settingsdialog.h

SOURCES += soxresampler.cpp \
           effectsoxrfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_LIB_PATH/libsoxr/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libsoxr/lib -lsoxr
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libsoxr/lib -lsoxr$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lsoxr
}
