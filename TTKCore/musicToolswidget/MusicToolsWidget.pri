#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

INCLUDEPATH += $$PWD \
               $$PWD/core

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musicsongssummarizied.h \
    $$PWD/musictoolsetswidget.h \
    $$PWD/musicmydownloadrecordwidget.h \
    $$PWD/core/musicmydownloadrecordconfigmanager.h \
    $$PWD/core/musicwebentainradiodatebase.h \
    $$PWD/musicconnectmobilewidget.h \
    $$PWD/musicwebradiotoolwidget.h \
    $$PWD/musicwebentainradiowidget.h \
    $$PWD/musicwebentainradiolistview.h \
    $$PWD/musicwebmusicradiolistview.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicsongssummarizied.cpp \
    $$PWD/musictoolsetswidget.cpp \
    $$PWD/musicmydownloadrecordwidget.cpp \
    $$PWD/core/musicmydownloadrecordconfigmanager.cpp \
    $$PWD/core/musicwebentainradiodatebase.cpp \
    $$PWD/musicconnectmobilewidget.cpp \
    $$PWD/musicwebradiotoolwidget.cpp \
    $$PWD/musicwebentainradiowidget.cpp \
    $$PWD/musicwebentainradiolistview.cpp \
    $$PWD/musicwebmusicradiolistview.cpp

}
