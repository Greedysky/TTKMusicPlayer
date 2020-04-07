include($$PWD/../../plugins.pri)

QT += opengl
DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += projectmplugin.h \
           projectmwidget.h \
           visualfloridprojectmfactory.h

SOURCES += projectmplugin.cpp \
           visualfloridprojectmfactory.cpp \
           projectmwidget.cpp

# Disbale gcc warnings
QMAKE_CFLAGS += -w
QMAKE_CXXFLAGS += -w

INCLUDEPATH += $$EXTRA_PREFIX/libprojectm/include

win32:{
    LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM.dll -lglu32 -lopengl32
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM$$STATIC_LIBRARY_SUFFIX
}
