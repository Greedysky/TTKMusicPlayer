include($$PWD/../../Visual.pri)

lessThan(QT_MAJOR_VERSION, 6){
    QT += opengl
}else{
    QT += openglwidgets
}

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += madspinplugin.h \
           madspinwidget.h \
           visualflowmadspinfactory.h

SOURCES += madspinplugin.cpp \
           madspinwidget.cpp \
           visualflowmadspinfactory.cpp

RESOURCES += flowmadspin.qrc

win32{
    LIBS += -lglu32 -lopengl32
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
}
