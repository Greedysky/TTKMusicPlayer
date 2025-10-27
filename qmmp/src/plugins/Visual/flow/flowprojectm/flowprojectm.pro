include($$PWD/../../Visual.pri)

#DEFINES += PROJECTM_31
#DEFINES += PROJECTM_4

lessThan(QT_MAJOR_VERSION, 6){
    QT += opengl
}else{
    QT += openglwidgets
}

DESTDIR = $$PLUGINS_PREFIX/Visual

contains(DEFINES, PROJECTM_4) {
    HEADERS += projectm4widget.h
    SOURCES += projectm4widget.cpp
} else {
    HEADERS += projectmwidget.h
    SOURCES += projectmwidget.cpp
}

HEADERS += projectmhelper.h \
           projectmplugin.h \
           visualflowprojectmfactory.h

SOURCES += projectmhelper.cpp \
           projectmplugin.cpp \
           visualflowprojectmfactory.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libprojectm/include

win32{
    contains(DEFINES, PROJECTM_4){
        LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM-4 -lprojectM-4-playlist
    }else{
        LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM
    }
    LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lglu32 -lopengl32
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    contains(DEFINES, PROJECTM_4){
        LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM-4$$STATIC_LIBRARY_SUFFIX -lprojectM-4-playlist$$STATIC_LIBRARY_SUFFIX
    }else{
        LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM$$STATIC_LIBRARY_SUFFIX
    }
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    contains(DEFINES, PROJECTM_4){
        LIBS += -lprojectM-4 -lprojectM-4-playlist
    }else{
        LIBS += -lprojectM
    }
}
