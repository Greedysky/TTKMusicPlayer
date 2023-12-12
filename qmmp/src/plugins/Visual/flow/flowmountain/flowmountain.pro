include($$PWD/../../Visual.pri)

QT += opengl
DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += mountainplugin.h \
           mountainwidget.h \
           visualflowmountainfactory.h

SOURCES += mountainplugin.cpp \
           mountainwidget.cpp \
           visualflowmountainfactory.cpp

win32{
    LIBS += -lglu32 -lopengl32
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
