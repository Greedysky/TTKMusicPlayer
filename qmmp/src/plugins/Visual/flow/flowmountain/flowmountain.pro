include($$PWD/../../Visual.pri)

lessThan(QT_MAJOR_VERSION, 6){
    QT += opengl
}else{
    QT += openglwidgets
}

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += mountainplugin.h \
           mountainwidget.h \
           mountainmidwidget.h \
           mountainsidewidget.h \
           visualflowmountainfactory.h

SOURCES += mountainplugin.cpp \
           mountainwidget.cpp \
           mountainmidwidget.cpp \
           mountainsidewidget.cpp \
           visualflowmountainfactory.cpp

win32{
    LIBS += -lglu32 -lopengl32
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
