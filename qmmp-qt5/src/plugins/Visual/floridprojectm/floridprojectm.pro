include($$PWD/../../plugins.pri)

QT += opengl
DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = floridprojectm

HEADERS += projectmplugin.h \
           projectmwidget.h \
           visualfloridprojectmfactory.h

SOURCES += projectmplugin.cpp \
           projectmwidget.cpp \
           visualfloridprojectmfactory.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libprojectm/include

win32:{
    LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM.dll -lglu32 -lopengl32
}

unix{
    QMAKE_CLEAN = libfloridprojectm.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM$$STATIC_LIBRARY_SUFFIX
}
