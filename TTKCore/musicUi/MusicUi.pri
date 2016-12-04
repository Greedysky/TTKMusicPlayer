#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2014 - 2016 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

INCLUDEPATH += $$PWD

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musicuiobject.h \
    $$PWD/musiccontextuiobject.h \
    $$PWD/musicdesktoplrcuiobject.h \
    $$PWD/musicfunctionlistuiobject.h \
    $$PWD/musicfunctionuiobject.h \
    $$PWD/musicinlinefloatuiobject.h \
    $$PWD/musicinlinelrcuiobject.h \
    $$PWD/musicleftitemlistuiobject.h \
    $$PWD/musicmagicwidgetuiobject.h \
    $$PWD/musicqualitywidgetuiobject.h \
    $$PWD/musictinyuiobject.h \
    $$PWD/musictoolsetsuiobject.h \
    $$PWD/musicvideofloatuiobject.h \
    $$PWD/musicvideouiobject.h
}

FORMS    += \
    $$PWD/musicApplication/musicapplication.ui \
    $$PWD/musicLrcmanager/musiclrcartphotouploadwidget.ui \
    $$PWD/musicLrcmanager/musiclrcmakerwidget.ui \
    $$PWD/musicLrcmanager/musiclrcerrorwidget.ui \
    $$PWD/musicLrcmanager/musiclrcsearchwidget.ui \
    $$PWD/musicLrcmanager/musiclrclocallinkwidget.ui \
    $$PWD/musicLrcmanager/musiclrccolorwidget.ui \
    $$PWD/musicToolsetswidget/musicaudiorecorderwidget.ui \
    $$PWD/musicToolsetswidget/musicdesktopwallpaperwidget.ui \
    $$PWD/musicToolsetswidget/musicfileinformationwidget.ui \
    $$PWD/musicToolsetswidget/musiclocalsongsmanagerwidget.ui \
    $$PWD/musicToolsetswidget/musictimerwidget.ui \
    $$PWD/musicToolsetswidget/musictransformwidget.ui \
    $$PWD/musicToolsetswidget/musicnetworkspeedtestwidget.ui \
    $$PWD/musicToolsetswidget/musicnetworkconnectiontestwidget.ui \
    $$PWD/musicToolsetswidget/musicvolumegainwidget.ui \
    $$PWD/musicToolsetswidget/musicsoundtouchwidget.ui \
    $$PWD/musicToolsetswidget/musicsongchecktoolswidget.ui \
    $$PWD/musicToolsetswidget/musicsongringtonemakerwidget.ui \
    $$PWD/musicToolswidget/musicwebentainradiowidget.ui \
    $$PWD/musicToolswidget/musicwebmusicradiowidget.ui \
    $$PWD/musicToolswidget/musicconnecttransferwidget.ui \
    $$PWD/musicToolswidget/musiccloudfilemanagerdialog.ui \
    $$PWD/musicUsermanager/musicuserdialog.ui \
    $$PWD/musicUsermanager/musicusermanagerdialog.ui \
    $$PWD/musicUsermanager/musicuserrecordwidget.ui \
    $$PWD/musicUsermanager/musicuserwindow.ui \
    $$PWD/musicWidget/musicbackgroundskindialog.ui \
    $$PWD/musicWidget/musicbackgroundpalettewidget.ui \
    $$PWD/musicWidget/musicdownloadwidget.ui \
    $$PWD/musicWidget/musicdownloadresetwidget.ui \
    $$PWD/musicWidget/musicequalizerdialog.ui \
    $$PWD/musicWidget/musicmessagebox.ui \
    $$PWD/musicWidget/musicsettingwidget.ui \
    $$PWD/musicWidget/musicsongsharingwidget.ui \
    $$PWD/musicWidget/musicsonglistsharingwidget.ui \
    $$PWD/musicWidget/musicsongslistiteminfowidget.ui \
    $$PWD/musicWidget/musicsourceupdatewidget.ui \
    $$PWD/musicWidget/musicsoundeffectswidget.ui
