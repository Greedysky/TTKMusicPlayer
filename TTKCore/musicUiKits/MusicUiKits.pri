# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
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
    $$PWD/musicLrcKits/musiclrcartphotouploadwidget.ui \
    $$PWD/musicLrcKits/musiclrcmakerwidget.ui \
    $$PWD/musicLrcKits/musiclrcerrorwidget.ui \
    $$PWD/musicLrcKits/musiclrcsearchwidget.ui \
    $$PWD/musicLrcKits/musiclrclocallinkwidget.ui \
    $$PWD/musicLrcKits/musiclrccolorwidget.ui \
    $$PWD/musicToolsetsKits/musicaudiorecorderwidget.ui \
    $$PWD/musicToolsetsKits/musicdesktopwallpaperwidget.ui \
    $$PWD/musicToolsetsKits/musicfileinformationwidget.ui \
    $$PWD/musicToolsetsKits/musiclocalsongsmanagerwidget.ui \
    $$PWD/musicToolsetsKits/musictimerwidget.ui \
    $$PWD/musicToolsetsKits/musictransformwidget.ui \
    $$PWD/musicToolsetsKits/musicnetworkspeedtestwidget.ui \
    $$PWD/musicToolsetsKits/musicnetworkconnectiontestwidget.ui \
    $$PWD/musicToolsetsKits/musicvolumegainwidget.ui \
    $$PWD/musicToolsetsKits/musicsoundtouchwidget.ui \
    $$PWD/musicToolsetsKits/musicsongchecktoolswidget.ui \
    $$PWD/musicToolsetsKits/musicsongringtonemakerwidget.ui \
    $$PWD/musicToolsetsKits/musicsongchecktoolsitemselecteddialog.ui \
    $$PWD/musicToolsetsKits/musicsoundkmicrowidget.ui \
    $$PWD/musicToolsKits/musicwebmusicradiowidget.ui \
    $$PWD/musicToolsKits/musicconnecttransferwidget.ui \
    $$PWD/musicToolsKits/musiccloudfilemanagerdialog.ui \
    $$PWD/musicUserKits/musicuserdialog.ui \
    $$PWD/musicUserKits/musicusermanagerdialog.ui \
    $$PWD/musicUserKits/musicuserrecordwidget.ui \
    $$PWD/musicUserKits/musicuserwindow.ui \
    $$PWD/musicWidgetKits/musicbackgroundskindialog.ui \
    $$PWD/musicWidgetKits/musicbackgroundpalettewidget.ui \
    $$PWD/musicWidgetKits/musicdownloadwidget.ui \
    $$PWD/musicWidgetKits/musicdownloadresetwidget.ui \
    $$PWD/musicWidgetKits/musicequalizerdialog.ui \
    $$PWD/musicWidgetKits/musicmessagebox.ui \
    $$PWD/musicWidgetKits/musicsettingwidget.ui \
    $$PWD/musicWidgetKits/musicsongsharingwidget.ui \
    $$PWD/musicWidgetKits/musicsonglistsharingwidget.ui \
    $$PWD/musicWidgetKits/musicsongslistiteminfowidget.ui \
    $$PWD/musicWidgetKits/musicsourceupdatewidget.ui \
    $$PWD/musicWidgetKits/musicsoundeffectswidget.ui \
    $$PWD/musicWidgetKits/musicmessageaboutdialog.ui \
    $$PWD/musicWidgetKits/musicsonglistenhancelosslesswidget.ui
