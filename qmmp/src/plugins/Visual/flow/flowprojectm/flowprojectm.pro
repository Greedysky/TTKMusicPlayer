include($$PWD/../../Visual.pri)

# Check version
mac{
    LIB_PKG_PATH = /opt/local/lib/pkgconfig/libprojectM.pc
}else{
    LIB_PKG_PATH = $$EXTRA_PREFIX/libprojectm/lib/pkgconfig/libprojectM.pc
}

exists($$LIB_PKG_PATH){
    win32{
        LIB_VERSION = $$system(findstr /r "Version:" $$LIB_PKG_PATH)
        !isEmpty(LIB_VERSION){
            LIB_VERSION = $$split(LIB_VERSION, " ")
            LIB_VERSION = $$member(LIB_VERSION, 1)
        }
    }
    unix:LIB_VERSION = $$system(grep "Version:" $$LIB_PKG_PATH | sed \"s/Version: //g\")
}

isEmpty(LIB_VERSION){
    message("Found libprojectM version 2.x")
}else{
    LIB_VERSION = $$split(LIB_VERSION, ".")
    LIB_VERSION = $$member(LIB_VERSION, 0)

    greaterThan(LIB_VERSION, 3){
        DEFINES += PROJECTM_4
        message("Found libprojectM version 4.x")
    }else{
        equals(LIB_VERSION, 3){
            DEFINES += PROJECTM_31
            message("Found libprojectM version 3.x")
        }else{
            message("Found libprojectM version 2.x")
        }
    }
}

freebsd:DEFINES += PROJECTM_31

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
