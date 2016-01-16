#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

INCLUDEPATH += $$PWD \
               $$PWD/network

!contains(CONFIG, TTK_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musiclogger.h \
    $$PWD/musiclibexportglobal.h \
    $$PWD/musicobject.h \
    $$PWD/musicuiobject.h \
    $$PWD/network/musicdownloadthreadabstract.h \
    $$PWD/network/musicdata2downloadthread.h \
    $$PWD/network/musicdatadownloadthread.h \
    $$PWD/network/musictextdownloadthread.h \
    $$PWD/network/musicbgthemedownload.h \
    $$PWD/network/musicnetworkthread.h \
    $$PWD/network/musicdownloadqueuecache.h \
    $$PWD/network/musicdownloadquerythreadabstract.h \
    $$PWD/network/musicdownloadquerysinglethread.h \
    $$PWD/network/musicdownloadquerymultiplethread.h \
    $$PWD/musicversion.h \
    $$PWD/musictime.h \
    $$PWD/musicxmlconfigmanager.h \
    $$PWD/musicplayer.h \
    $$PWD/musicplaylist.h \
    $$PWD/musicequalizer.h \
    $$PWD/musicabstractxml.h \
    $$PWD/musiclibzplay.h \
    $$PWD/musicbgthememanager.h \
    $$PWD/musicsettingmanager.h \
    $$PWD/musicconnectionpool.h \
    $$PWD/musicregeditmanager.h \
    $$PWD/musicsingleton.h \
    $$PWD/musiccoremplayer.h \
    $$PWD/musicsong.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/network/musicdownloadthreadabstract.cpp \
    $$PWD/network/musicdata2downloadthread.cpp \
    $$PWD/network/musicdatadownloadthread.cpp \
    $$PWD/network/musictextdownloadthread.cpp \
    $$PWD/network/musicbgthemedownload.cpp \
    $$PWD/network/musicnetworkthread.cpp \
    $$PWD/network/musicdownloadqueuecache.cpp \
    $$PWD/network/musicdownloadquerythreadabstract.cpp \
    $$PWD/network/musicdownloadquerysinglethread.cpp \
    $$PWD/network/musicdownloadquerymultiplethread.cpp \
    $$PWD/musictime.cpp \
    $$PWD/musicplayer.cpp \
    $$PWD/musicplaylist.cpp \
    $$PWD/musicequalizer.cpp \
    $$PWD/musicabstractxml.cpp \
    $$PWD/musicxmlconfigmanager.cpp \
    $$PWD/musicbgthememanager.cpp \
    $$PWD/musicconnectionpool.cpp \
    $$PWD/musicregeditmanager.cpp \
    $$PWD/musiccoremplayer.cpp \
    $$PWD/musicsong.cpp

}
