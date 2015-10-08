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
    $$PWD/core/musicuserconfigmanager.h
    
    
SOURCES += \
    $$PWD/musicusermanager.cpp \
    $$PWD/musicuserdialog.cpp \
    $$PWD/musicuserlineedit.cpp \
    $$PWD/musicuserwindow.cpp \
    $$PWD/core/musicusermodel.cpp \
    $$PWD/core/musicuserconfigmanager.cpp
    
    
FORMS    += \
    ui/usermanager/musicuserdialog.ui \
    ui/usermanager/musicuserwindow.ui \
    ui/usermanager/musicusermanager.ui

RESOURCES += \
    qrc/MusicPlayerShare.qrc \
    qrc/MusicPlayerUser.qrc \
