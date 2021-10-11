include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Light
TARGET = $${TARGET}

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

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag
}
