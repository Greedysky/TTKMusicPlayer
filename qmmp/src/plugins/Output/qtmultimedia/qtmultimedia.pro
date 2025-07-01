include($$PWD/../../plugins.pri)

unix:!mac{
    equals(QT_MAJOR_VERSION, 4){
        QMAKE_CXXFLAGS += -I/usr/include/QtMultimediaKit -I/usr/include/QtMobility
        LIBS += -lQtMultimediaKit
    }else{
        QT += multimedia
    }
}
win32:QT += multimedia

DESTDIR = $$PLUGINS_PREFIX/Output

HEADERS += outputqtmultimediafactory.h \
           outputqtmultimedia.h \
           settingsdialog.h

SOURCES += outputqtmultimediafactory.cpp \
           outputqtmultimedia.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

unix:QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
