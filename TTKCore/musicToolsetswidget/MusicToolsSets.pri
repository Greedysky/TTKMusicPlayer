#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

INCLUDEPATH += $$PWD \
               $$PWD/core

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musicmobiledeviceswidget.h \
    $$PWD/musicaudiorecorderwidget.h \
    $$PWD/musictimerwidget.h \
    $$PWD/musiclocalsongsmanagerwidget.h \
    $$PWD/musictransformwidget.h \
    $$PWD/musicfileinformationwidget.h \
    $$PWD/musicdesktopwallpaperwidget.h \
    $$PWD/core/musicdesktopwallpaperthread.h \
    $$PWD/core/musictimerautoobject.h \
    $$PWD/core/musiclocalsongsmanagerthread.h \
    $$PWD/core/musicsongtag.h \
    $$PWD/core/musicnetworktestthread.h \
    $$PWD/musiclocalsongstablewidget.h \
    $$PWD/musicspeedmeter.h \
    $$PWD/musicnetworktestwidget.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicmobiledeviceswidget.cpp \
    $$PWD/musicaudiorecorderwidget.cpp \
    $$PWD/musictimerwidget.cpp \
    $$PWD/musiclocalsongsmanagerwidget.cpp \
    $$PWD/musictransformwidget.cpp \
    $$PWD/musicfileinformationwidget.cpp \
    $$PWD/musicdesktopwallpaperwidget.cpp \
    $$PWD/core/musicdesktopwallpaperthread.cpp \
    $$PWD/core/musictimerautoobject.cpp \
    $$PWD/core/musiclocalsongsmanagerthread.cpp \
    $$PWD/core/musicsongtag.cpp \
    $$PWD/core/musicnetworktestthread.cpp \
    $$PWD/musiclocalsongstablewidget.cpp \
    $$PWD/musicspeedmeter.cpp \
    $$PWD/musicnetworktestwidget.cpp

}
