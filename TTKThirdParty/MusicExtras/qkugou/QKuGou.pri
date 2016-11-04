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

equals(QT_MAJOR_VERSION, 5){
    win32{
        msvc{
            exists($$[QT_INSTALL_LIBS]/libQt5WebKit.lib){
                QT       += webkit webkitwidgets
                DEFINES  += MUSIC_WEBKIT
            }
        }

        gcc{
            exists($$[QT_INSTALL_LIBS]/libQt5WebKit.a){
                QT       += webkit webkitwidgets
                DEFINES  += MUSIC_WEBKIT
            }
        }
    }

    unix:!mac{
        exists($$[QT_INSTALL_LIBS]/libQt5WebKit.so){
            QT       += webkit webkitwidgets
            DEFINES  += MUSIC_WEBKIT
        }
    }
}
else{
    QT       += webkit webkitwidgets
    DEFINES  += MUSIC_WEBKIT
}

contains(QT, webkit){
    message("Found webkit component, build in KuGou by webkit")
}
else{
    message("Not found webkit component, build in KuGou by normal")
}

INCLUDEPATH += $$PWD

HEADERS  += \
    $$PWD/kugouurl.h \
    $$PWD/kugouwindow.h \
    $$PWD/kugouuiobject.h

SOURCES += \
    $$PWD/kugouurl.cpp \
    $$PWD/kugouwindow.cpp
