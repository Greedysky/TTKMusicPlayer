include($$PWD/../../Visual.pri)

QT += opengl
DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += projectmplugin.h \
           projectmwidget.h \
           visualflowprojectmfactory.h

SOURCES += projectmplugin.cpp \
           projectmwidget.cpp \
           visualflowprojectmfactory.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libprojectm/include

win32{
    LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM.dll -lglu32 -lopengl32
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM$$STATIC_LIBRARY_SUFFIX
}
