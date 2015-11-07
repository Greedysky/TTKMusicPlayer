#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

INCLUDEPATH += $$PWD \
               $$PWD/core
               
HEADERS  += \
    $$PWD/musicusermanager.h \
    $$PWD/musicuserdialog.h \
    $$PWD/musicuserlineedit.h \
    $$PWD/musicuserwindow.h \
    $$PWD/core/musicusermodel.h \
    $$PWD/core/musicuserconfigmanager.h \
    $$PWD/musicuserrecordwidget.h
    
    
SOURCES += \
    $$PWD/musicusermanager.cpp \
    $$PWD/musicuserdialog.cpp \
    $$PWD/musicuserlineedit.cpp \
    $$PWD/musicuserwindow.cpp \
    $$PWD/core/musicusermodel.cpp \
    $$PWD/core/musicuserconfigmanager.cpp \
    $$PWD/musicuserrecordwidget.cpp
    
    
FORMS    += \
    musicUi/musicUsermanager/musicuserdialog.ui \
    musicUi/musicUsermanager/musicuserwindow.ui \
    musicUi/musicUsermanager/musicusermanager.ui \
    musicUi/musicUsermanager/musicuserrecordwidget.ui

RESOURCES += \
    musicQrc/MusicPlayerShare.qrc \
    musicQrc/MusicPlayerUser.qrc \
