include(../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Visual/floridprojectm

HEADERS += projectmplugin.h \
           projectmwidget.h \
           visualfloridprojectmfactory.h

SOURCES += projectmplugin.cpp \
           projectmwidget.cpp \
           visualfloridprojectmfactory.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libprojectm/include

win32:{
    HEADERS += ../../../../src/qmmp/florid.h
    INCLUDEPATH += ./
    LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/floridprojectm.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libprojectm/lib -lprojectM$$STATIC_LIBRARY_SUFFIX
}
