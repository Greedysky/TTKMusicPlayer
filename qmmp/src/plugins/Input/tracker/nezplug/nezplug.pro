include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decodernezplugfactory.h \
           decoder_nezplug.h \
           nezplughelper.h \
           settingsdialog.h

SOURCES += decodernezplugfactory.cpp \
           decoder_nezplug.cpp \
           nezplughelper.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_LIB_PATH/libttk/include \
               $$EXTRA_LIB_PATH/libttk/include/libnezplug

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lnezplug
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lnezplug$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lnezplug
}
