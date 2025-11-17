include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += goomwidget.h \
           visualflowgoomfactory.h

SOURCES += goomwidget.cpp \
           visualflowgoomfactory.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libttk/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lgoom
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libttk/lib -lgoom$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lgoom
}
