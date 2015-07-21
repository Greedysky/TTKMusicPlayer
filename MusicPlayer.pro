#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

QT       += core gui xml sql
QT       += multimedia multimediawidgets
QT       += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayer
#TARGET = MusicUser
#TARGET = MusicVideo

TEMPLATE = app
#LIBS += -L"./" -lMusicPlayer -lMusicUser -lMusicVideo
#LIBS += -L"./" -lMusicUser  -lMusicVideo
#TEMPLATE = lib
LIBS += -L"./MPlugins/" -llibzplay

DEFINES += MUSIC_LIBRARY

TRANSLATIONS += language/cn.ts \
                language/cn_c.ts \
                language/en.ts

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

SOURCES += \
    musicapplicationmain.cpp\
    core/musicmovedialogabstract.cpp \
    core/musicmovewidgetabstract.cpp \
    musicapplication.cpp \
    musicsongslistwidget.cpp \
    musicmarquee.cpp \
    musicsongslistiteminformation.cpp \
    musicsettingwidget.cpp \
    musicbackgroundskindialog.cpp \
    musicsongsearchonlinewidget.cpp \
    musicsongstoolitemrenamedwidget.cpp \
    musicsystemtraymenu.cpp \
    musicpreviewlabel.cpp \
    musicequalizerdialog.cpp \
    musicsongslistplaywidget.cpp \
    musictimersliderwidget.cpp \
    musicwindowextras.cpp \
    localsearch/musiclocalsongsearch.cpp \
    localsearch/musiclocalsongsearchedit.cpp \
    localsearch/musiclocalsongsearchinlineedit.cpp \
    localsearch/musiclocalsongsearchpopwidget.cpp \
    localsearch/musiclocalsongsearchrecordobject.cpp \
    lrcmanager/musiclrcmanager.cpp \
    lrcmanager/musiclrcmanagerfordesktop.cpp \
    lrcmanager/musiclrcmanagerforinline.cpp \
    lrcmanager/musiclrccontainer.cpp \
    lrcmanager/musiclrccontainerfordesktop.cpp \
    lrcmanager/musiclrccontainerforinline.cpp \
    lrcmanager/musiclrcsearchwidget.cpp \
    lrcmanager/musiclrcsearchtablewidget.cpp \
    lrcmanager/musiclrcartphotoupload.cpp \
    lrcmanager/musiclrcartphotolabel.cpp \
    lrcmanager/musiclrcfloatwidget.cpp \
    remotewidget/musicremotewidget.cpp \
    remotewidget/musicremotewidgetforsquare.cpp \
    remotewidget/musicremotewidgetforrectangle.cpp \
    remotewidget/musicremotewidgetfordiamond.cpp \
    remotewidget/musicremotewidgetforcircle.cpp \
    toolswidget/musicsongssummarizied.cpp \
    toolswidget/musictoolsetswidget.cpp \
    toolswidget/musicmydownloadrecordwidget.cpp \
    toolswidget/musicwebradiolistview.cpp \
    toolswidget/musicwebradiowidget.cpp \
    toolswidget/musicwebradiodatebase.cpp \
    toolswidget/musicmydownloadrecordobject.cpp \
    toolsetswidget/musicmobiledeviceswidget.cpp \
    toolsetswidget/musicaudiorecorderwidget.cpp \
    toolsetswidget/musictimerwidget.cpp \
    toolsetswidget/musiclocalsongsmanagerwidget.cpp \
    toolsetswidget/musictransformwidget.cpp \
    toolsetswidget/musicfileinformationwidget.cpp \
    toolsetswidget/musicspectrumwidget.cpp \
    toolsetswidget/musicdesktopwallpaperwidget.cpp \
    core/musicplayer.cpp \
    core/musicplaylist.cpp \
    core/musicequalizer.cpp \
    core/musicxmlabstract.cpp \
    core/musictimerautoobject.cpp \
    core/musicdownloadmanagerthread.cpp \
    core/musicdownloadthreadabstract.cpp \
    core/musicdownloadstatuslabel.cpp \
    core/musicdatadownloadthread.cpp \
    core/musicsongdownloadthread.cpp \
    core/musiclrcdownloadthread.cpp \
    core/musicxmlconfigmanager.cpp \
    core/musiclocalsongsmanagerthread.cpp \
    core/musicfileinformation.cpp \
    core/musicbgthemedownload.cpp \
    core/musicdesktopwallpaperthread.cpp \
    core/musictablewidgetabstract.cpp


HEADERS  += \
#    core/musiclogger.h \
    core/musiclibexportglobal.h \
    core/musicobject.h \
    core/musicmovedialogabstract.h \
    core/musicmovewidgetabstract.h \
    musicapplication.h \
    musicsongslistwidget.h \
    musicmarquee.h \
    musicsongslistiteminformation.h \
    musicsettingwidget.h \
    musicbackgroundskindialog.h \
    musicsongsearchonlinewidget.h \
    musicsongstoolitemrenamedwidget.h \
    musicsystemtraymenu.h \
    musicpreviewlabel.h \
    musicequalizerdialog.h \
    musicsongslistplaywidget.h \
    musictimersliderwidget.h \
    musicwindowextras.h \
    localsearch/musiclocalsongsearch.h \
    localsearch/musiclocalsongsearchedit.h \
    localsearch/musiclocalsongsearchinlineedit.h \
    localsearch/musiclocalsongsearchpopwidget.h \
    localsearch/musiclocalsongsearchrecordobject.h \
    lrcmanager/musiclrcmanager.h \
    lrcmanager/musiclrcmanagerfordesktop.h \
    lrcmanager/musiclrcmanagerforinline.h \
    lrcmanager/musiclrccontainer.h \
    lrcmanager/musiclrccontainerfordesktop.h \
    lrcmanager/musiclrccontainerforinline.h \
    lrcmanager/musiclrcsearchwidget.h \
    lrcmanager/musiclrcsearchtablewidget.h \
    lrcmanager/musiclrcartphotoupload.h \
    lrcmanager/musiclrcartphotolabel.h \
    lrcmanager/musiclrcfloatwidget.h \
    remotewidget/musicremotewidget.h \
    remotewidget/musicremotewidgetforsquare.h \
    remotewidget/musicremotewidgetforrectangle.h \
    remotewidget/musicremotewidgetfordiamond.h \
    remotewidget/musicremotewidgetforcircle.h \
    toolswidget/musicsongssummarizied.h \
    toolswidget/musictoolsetswidget.h \
    toolswidget/musicmydownloadrecordwidget.h \
    toolswidget/musicwebradiolistview.h \
    toolswidget/musicwebradiowidget.h \
    toolswidget/musicwebradiodatebase.h \
    toolswidget/musicmydownloadrecordobject.h \
    toolsetswidget/musicmobiledeviceswidget.h \
    toolsetswidget/musicaudiorecorderwidget.h \
    toolsetswidget/musictimerwidget.h \
    toolsetswidget/musiclocalsongsmanagerwidget.h \
    toolsetswidget/musictransformwidget.h \
    toolsetswidget/musicfileinformationwidget.h \
    toolsetswidget/musicspectrumwidget.h \
    toolsetswidget/musicdesktopwallpaperwidget.h \
    core/musicversion.h \
    core/musicsonginformation.h \
    core/musicxmlconfigmanager.h \
    core/musicdownloadmanagerthread.h \
    core/musicdownloadthreadabstract.h \
    core/musicdatadownloadthread.h \
    core/musicsongdownloadthread.h \
    core/musiclrcdownloadthread.h \
    core/musicdownloadstatuslabel.h \
    core/musicplayer.h \
    core/musicplaylist.h \
    core/musicequalizer.h \
    core/musicxmlabstract.h \
    core/musictimerautoobject.h \
    core/musiclocalsongsmanagerthread.h \
    core/musiclibzplay.h \
    core/musicfileinformation.h \
    core/musicbgthemedownload.h \
    core/musicdesktopwallpaperthread.h \
    core/musictablewidgetabstract.h \
    usermanager/musicusermanager.h \
    usermanager/musicusermodel.h \
    usermanager/musicuserdialog.h \
    usermanager/musicuserlineedit.h \
    usermanager/musicuserwindow.h \
    usermanager/musicuserconfigmanager.h \
    videokits/musicvideoplayer.h \
    videokits/musicvideocontrol.h \
    videokits/musicvideoview.h \
    videokits/musicvideotablewidget.h


FORMS    += \
    ui/application/musicapplication.ui \
    ui/application/musicsongslistiteminformation.ui \
    ui/application/musicsettingwidget.ui \
    ui/application/musicbackgroundskindialog.ui \
    ui/application/musicequalizerdialog.ui \
    ui/application/musicaudiorecorderwidget.ui \
    ui/application/musictimerwidget.ui \
    ui/application/musiclocalsongsmanagerwidget.ui \
    ui/application/musictransformwidget.ui \
    ui/application/musicwebradiowidget.ui \
    ui/application/musicfileinformationwidget.ui \
    ui/application/musicdesktopwallpaperwidget.ui \
    ui/application/musiclrcsearchwidget.ui \
    ui/application/musiclrcartphotoupload.ui

RESOURCES += \
    qrc/MusicPlayerShare.qrc \
    qrc/MusicPlayer.qrc \

RC_FILE = \
    MusicPlayer.rc

include(usermanager/MusicUser.pri)
include(videokits/MusicVideo.pri)


