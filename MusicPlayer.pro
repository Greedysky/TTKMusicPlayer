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
#TARGET = MusicCore

TEMPLATE = app
#LIBS += -L"./" -lMusicCore
#TEMPLATE = lib
LIBS += -L"./MPlugins/" -llibzplay

DEFINES += MUSIC_LIBRARY

INCLUDEPATH += $$PWD/core \
               $$PWD/core-widget \
               $$PWD/localsearch \
               $$PWD/lrcmanager \
               $$PWD/remotewidget \
               $$PWD/toolsetswidget \
               $$PWD/toolswidget \
               $$PWD/usermanager \
               $$PWD/videokits \
               $$PWD/

TRANSLATIONS += language/cn.ts \
                language/cn_c.ts \
                language/en.ts

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

SOURCES += \
    musicapplicationmain.cpp\
    musicapplication.cpp \
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
    lrcmanager/musiclrcfloatabstractwidget.cpp \
    lrcmanager/musiclrcfloatsettingwidget.cpp \
    lrcmanager/musiclrcfloatphotowidget.cpp \
    lrcmanager/musiclrcmakerwidget.cpp \
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
    core/musictime.cpp \
    core/musicplayer.cpp \
    core/musicplaylist.cpp \
    core/musicequalizer.cpp \
    core/musicabstractxml.cpp \
    core/musictimerautoobject.cpp \
    core/musicdownloadquerythread.cpp \
    core/musicdownloadthreadabstract.cpp \
    core/musicdata2downloadthread.cpp \
    core/musicdatadownloadthread.cpp \
    core/musictextdownloadthread.cpp \
    core/musicxmlconfigmanager.cpp \
    core/musiclocalsongsmanagerthread.cpp \
    core/musicfileinformation.cpp \
    core/musicbgthemedownload.cpp \
    core/musicbgthememanager.cpp \
    core/musicdesktopwallpaperthread.cpp \
    core/musicconnectionpool.cpp \
    core-widget/musicdownloadstatuslabel.cpp \
    core-widget/musicabstracttablewidget.cpp \
    core-widget/musicquerytablewidget.cpp \
    core-widget/musiclocalsongstablewidget.cpp \
    core-widget/musicabstractmovedialog.cpp \
    core-widget/musicabstractmovewidget.cpp \
    core-widget/musicmarqueewidget.cpp \
    core-widget/musicsettingwidget.cpp \
    core-widget/musicbackgroundskindialog.cpp \
    core-widget/musicsystemtraymenu.cpp \
    core-widget/musicpreviewlabel.cpp \
    core-widget/musicequalizerdialog.cpp \
    core-widget/musictimersliderwidget.cpp \
    core-widget/musicwindowextras.cpp \
    core-widget/musicsongslistwidget.cpp \
    core-widget/musicsongslistiteminfowidget.cpp \
    core-widget/musicsongsearchonlinewidget.cpp \
    core-widget/musicsongstoolitemrenamedwidget.cpp \
    core-widget/musicsongslistplaywidget.cpp \
    core-widget/musicmessagebox.cpp \
    musicleftareawidget.cpp \
    musictopareawidget.cpp \
    musicrightareawidget.cpp \
    musicbottomareawidget.cpp \
    musicapplicationobject.cpp


HEADERS  += \
    core/musiclogger.h \
    core/musiclibexportglobal.h \
    core/musicobject.h \
    core/musicuiobject.h \
    core/musicsingletone.h \
    musicapplication.h \
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
    lrcmanager/musiclrcfloatabstractwidget.h \
    lrcmanager/musiclrcfloatsettingwidget.h \
    lrcmanager/musiclrcfloatphotowidget.h \
    lrcmanager/musiclrcmakerwidget.h \
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
    core/musictime.h \
    core/musicsonginformation.h \
    core/musicxmlconfigmanager.h \
    core/musicdownloadquerythread.h \
    core/musicdownloadthreadabstract.h \
    core/musicdata2downloadthread.h \
    core/musicdatadownloadthread.h \
    core/musictextdownloadthread.h \
    core/musicplayer.h \
    core/musicplaylist.h \
    core/musicequalizer.h \
    core/musicabstractxml.h \
    core/musictimerautoobject.h \
    core/musiclocalsongsmanagerthread.h \
    core/musiclibzplay.h \
    core/musicfileinformation.h \
    core/musicbgthemedownload.h \
    core/musicbgthememanager.h \
    core/musicsettingmanager.h \
    core/musicdesktopwallpaperthread.h \
    core/musicconnectionpool.h \
    core-widget/musicdownloadstatuslabel.h \
    core-widget/musicabstracttablewidget.h \
    core-widget/musicquerytablewidget.h \
    core-widget/musiclocalsongstablewidget.h \
    core-widget/musicabstractmovedialog.h \
    core-widget/musicabstractmovewidget.h \
    core-widget/musicmarqueewidget.h \
    core-widget/musicsettingwidget.h \
    core-widget/musicbackgroundskindialog.h \
    core-widget/musicsystemtraymenu.h \
    core-widget/musicpreviewlabel.h \
    core-widget/musicequalizerdialog.h \
    core-widget/musictimersliderwidget.h \
    core-widget/musicwindowextras.h \
    core-widget/musicsongslistwidget.h \
    core-widget/musicsongslistiteminfowidget.h \
    core-widget/musicsongsearchonlinewidget.h \
    core-widget/musicsongstoolitemrenamedwidget.h \
    core-widget/musicsongslistplaywidget.h \
    core-widget/musicmessagebox.h \
    musicleftareawidget.h \
    musictopareawidget.h \
    musicrightareawidget.h \
    musicbottomareawidget.h \
    musicapplicationobject.h


FORMS    += \
    ui/application/musicapplication.ui \
    ui/core-widget/musicmessagebox.ui \
    ui/core-widget/musicsongslistiteminfowidget.ui \
    ui/core-widget/musicsettingwidget.ui \
    ui/core-widget/musicbackgroundskindialog.ui \
    ui/core-widget/musicequalizerdialog.ui \
    ui/lrcmanager/musiclrcsearchwidget.ui \
    ui/lrcmanager/musiclrcartphotoupload.ui \
    ui/lrcmanager/musiclrcmakerwidget.ui \
    ui/toolsetswidget/musicaudiorecorderwidget.ui \
    ui/toolsetswidget/musictimerwidget.ui \
    ui/toolsetswidget/musiclocalsongsmanagerwidget.ui \
    ui/toolsetswidget/musictransformwidget.ui \
    ui/toolsetswidget/musicfileinformationwidget.ui \
    ui/toolsetswidget/musicdesktopwallpaperwidget.ui \
    ui/toolswidget/musicwebradiowidget.ui


RESOURCES += \
    qrc/MusicPlayerShare.qrc \
    qrc/MusicPlayer.qrc \

RC_FILE = \
    MusicPlayer.rc

include(usermanager/MusicUser.pri)
include(videokits/MusicVideo.pri)


