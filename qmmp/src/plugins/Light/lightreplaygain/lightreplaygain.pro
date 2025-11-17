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

INCLUDEPATH += $$EXTRA_LIB_PATH/libtaglib/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -ltag
}
