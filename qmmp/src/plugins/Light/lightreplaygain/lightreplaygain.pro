include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Light

HEADERS += replaygainfactory.h \
           replaygainwidget.h \
           gain_analysis.h \
           replaygainner.h

SOURCES += replaygainfactory.cpp \
           replaygainwidget.cpp \
           gain_analysis.c \
           replaygainner.cpp

FORMS += replaygainwidget.ui

INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -ltag
}
