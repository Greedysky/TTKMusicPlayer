include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodergmefactory.h \
           decoder_gme.h \
           gmehelper.h \
           settingsdialog.h

SOURCES += decodergmefactory.cpp \
           decoder_gme.cpp \
           gmehelper.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libgme/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libgme/lib -lgme
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libgme/lib -lgme$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lgme
}
