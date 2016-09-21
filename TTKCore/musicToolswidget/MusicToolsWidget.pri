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

INCLUDEPATH += $$PWD \
               $$PWD/core

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/core/musicmydownloadrecordconfigmanager.h \
    $$PWD/core/musicwebentainradiodatebase.h \
    $$PWD/core/musicfilesenderserver.h \
    $$PWD/core/musicfilereceiveserver.h \
    $$PWD/musicsongstoolboxwidget.h \
    $$PWD/musicsongssummarizied.h \
    $$PWD/musictoolsetswidget.h \
    $$PWD/musicmydownloadrecordwidget.h \
    $$PWD/musicconnectmobilewidget.h \
    $$PWD/musicconnecttransferwidget.h \
    $$PWD/musicconnecttransfertablewidget.h \
    $$PWD/musicwebradiotoolwidget.h \
    $$PWD/musicwebentainradiowidget.h \
    $$PWD/musicwebentainradiolistview.h \
    $$PWD/musicwebmusicradiolistview.h  \
    $$PWD/musicwebmusicradiowidget.h \
    $$PWD/musiccloudsharedsongwidget.h \
    $$PWD/musiccloudfilemanagerdialog.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/core/musicmydownloadrecordconfigmanager.cpp \
    $$PWD/core/musicwebentainradiodatebase.cpp \
    $$PWD/core/musicfilesenderserver.cpp \
    $$PWD/core/musicfilereceiveserver.cpp \
    $$PWD/musicsongstoolboxwidget.cpp \
    $$PWD/musicsongssummarizied.cpp \
    $$PWD/musictoolsetswidget.cpp \
    $$PWD/musicmydownloadrecordwidget.cpp \
    $$PWD/musicconnectmobilewidget.cpp \
    $$PWD/musicconnecttransferwidget.cpp \
    $$PWD/musicconnecttransfertablewidget.cpp \
    $$PWD/musicwebradiotoolwidget.cpp \
    $$PWD/musicwebentainradiowidget.cpp \
    $$PWD/musicwebentainradiolistview.cpp \
    $$PWD/musicwebmusicradiolistview.cpp \
    $$PWD/musicwebmusicradiowidget.cpp \
    $$PWD/musiccloudsharedsongwidget.cpp \
    $$PWD/musiccloudfilemanagerdialog.cpp

}
